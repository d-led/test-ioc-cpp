//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_ASIOHTTPSERVER_HPP_05152011
#define INCLUDED_ASIOHTTPSERVER_HPP_05152011

#include "HttpServer.hpp"
#include "ConfigItems.hpp"
#include "di/constructor.hpp"

class AsioHttpServer :
  public HttpServer
{
public:
  DI_CONSTRUCTOR(AsioHttpServer,(
                   ConfigItem<DocRoot> &,
                   ConfigItem<HttpPort> &
                   ));

  ~AsioHttpServer();

  virtual
  void
  start();

  virtual
  void
  stop();

private:
  struct  Impl;
  Impl   *m;
};

#endif // INCLUDED_ASIOHTTPSERVER_HPP_05152011
