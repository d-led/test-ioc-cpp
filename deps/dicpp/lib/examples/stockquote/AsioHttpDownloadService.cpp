//
// Copyright Somani Software Construction Inc. 2010
//

#include "AsioHttpDownloadService.hpp"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

AsioHttpDownloadService::AsioHttpDownloadService(
  boost::asio::io_service & service 
  ):
  m_service(service)
{}

std::string
AsioHttpDownloadService::sync_get(std::string const & sserver,
                                  std::string const & spath,
                                  std::map<std::string,std::string> const & query_params) const
{
  boost::asio::io_service &io_service = m_service;

  const char * server = sserver.c_str();
  std::string  path   = spath; // "/d/quotes.csv?s=" + symbol + "&f=l1";

  if(query_params.size())
  {
    path += "?";

    for(std::map<std::string,std::string>::const_iterator it = query_params.begin(),
          next = query_params.begin(),
          end = query_params.end();
        it != end;
        ++it)
    {
      path += it->first + "=" + it->second;
      ++next;
      if(next != end)
        path += "&";
    }
  }

  // Get a list of endpoints corresponding to the server name.
  tcp::resolver resolver(io_service);
  tcp::resolver::query query(server, "http");
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  tcp::resolver::iterator end;

  // Try each endpoint until we successfully establish a connection.
  tcp::socket socket(io_service);
  boost::system::error_code error = boost::asio::error::host_not_found;
  while (error && endpoint_iterator != end)
  {
    socket.close();
    socket.connect(*endpoint_iterator++, error);
  }
  if (error)
    throw boost::system::system_error(error);

  // Form the request. We specify the "Connection: close" header so that the
  // server will close the socket after transmitting the response. This will
  // allow us to treat all data up until the EOF as the content.
  boost::asio::streambuf request;
  std::ostream request_stream(&request);
  request_stream << "GET " << path << " HTTP/1.0\r\n";
  request_stream << "Host: " << server << "\r\n";
  request_stream << "Accept: */*\r\n";
  request_stream << "Connection: close\r\n\r\n";

  // Send the request.
  boost::asio::write(socket, request);

  // Read the response status line.
  boost::asio::streambuf response;
  boost::asio::read_until(socket, response, "\r\n");

  // Check that response is OK.
  std::istream response_stream(&response);
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message);
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    throw std::runtime_error("Invalid http response");
  if (status_code != 200)
    throw std::runtime_error("Unexpected HTTP response");

  // Read the response headers, which are terminated by a blank line.
  boost::asio::read_until(socket, response, "\r\n\r\n");

  // Process the response headers.
  std::ostringstream response_content;
  std::string header;
  while (std::getline(response_stream, header) && header != "\r");

  // Write whatever content we already have to output.
  if (response.size() > 0)
    response_content << &response;

  // Read until EOF, writing data to output as we go.
  while (boost::asio::read(socket, response,
                           boost::asio::transfer_at_least(1), error))
    response_content << &response;
  if (error != boost::asio::error::eof)
    throw boost::system::system_error(error);

  return response_content.str();
}

