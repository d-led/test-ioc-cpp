//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_HTTPSERVER_HPP_05152011
#define INCLUDED_HTTPSERVER_HPP_05152011

#include "boost/noncopyable.hpp"

// Really stupid HTTP server interface
struct HttpServer
{
  virtual ~HttpServer(){}

  virtual
  void
  start() = 0;

  virtual
  void
  stop() = 0;
};

#endif // INCLUDED_HTTPSERVER_HPP_05152011
