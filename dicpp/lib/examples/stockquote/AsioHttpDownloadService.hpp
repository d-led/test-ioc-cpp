//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_ASIOHTTPDOWNLOADSERVICE_HPP
#define INCLUDED_ASIOHTTPDOWNLOADSERVICE_HPP

#include "HttpDownloadService.hpp"
#include <boost/asio/io_service.hpp>
#include "di/constructor.hpp"
#include "di/type_provider.hpp"

class AsioHttpDownloadService : public HttpDownloadService
{
public:
  DI_CONSTRUCTOR(
    AsioHttpDownloadService,
    ( boost::asio::io_service & service )
    );
  
  virtual
  std::string
  sync_get(std::string const & server,
           std::string const & path,
           std::map<std::string,std::string> const & query_params) const;

private:
  boost::asio::io_service & m_service;
};

#endif // ASIOHTTPDOWNLOADSERVICE_HPP
