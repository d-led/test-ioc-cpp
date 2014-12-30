//
// Copyright Somani Software Construction Inc. (2011)
//

#include "AsioHttpServer.hpp"
#include "boost/asio/io_service.hpp"
#include "boost/thread.hpp"
#include "httpserver/server.hpp"
#include <memory>
#include <iostream>

struct AsioHttpServer::Impl
{
  boost::filesystem::path             docRoot;
  int                                 httpPort;
  std::auto_ptr<http::server::server> server;
  std::auto_ptr<boost::thread>        serverThread;

  void
  handlePortChange( int port, HttpServer & server )
  {
    server.stop();
    this->httpPort = port;
    server.start();
  }

  void
  handleDocrootChange( boost::filesystem::path const & docRoot, HttpServer & server)
  {
    server.stop();
    this->docRoot = docRoot;
    server.start();
  }

};

AsioHttpServer::AsioHttpServer( ConfigItem<DocRoot> & docRoot,
                                ConfigItem<HttpPort> & httpPort )
  : m (new Impl)
{
  m->docRoot  = docRoot.getValue();
  m->httpPort = httpPort.getValue();

  docRoot.signalValueChanged().connect(std::bind(&Impl::handleDocrootChange,m,_1,std::ref(*this)));
  httpPort.signalValueChanged().connect(std::bind(&Impl::handlePortChange,m,_1,std::ref(*this)));
}

AsioHttpServer::~AsioHttpServer()
{ delete m; }

void
AsioHttpServer::start()
{
  stop();
  std::ostringstream os;
  os << m->httpPort;
  m->server.reset(new http::server::server("0.0.0.0",
                                           os.str(),
                                           m->docRoot.string()));
  std::cout << "Starting server on port " << std::setbase(10) << m->httpPort
            << " serving from " << m->docRoot.file_string() << std::endl;
  m->serverThread.reset(new boost::thread(&http::server::server::run,m->server.get()));
}

void
AsioHttpServer::stop()
{
  if(!m->server.get() ||
     !m->serverThread.get() ||
     !m->serverThread->joinable())
    return;
  m->server->stop();
  m->serverThread->join();
}
