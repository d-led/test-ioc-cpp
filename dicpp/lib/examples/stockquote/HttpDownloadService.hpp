//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_HTTPDOWNLOADSERVICE_HPP
#define INCLUDED_HTTPDOWNLOADSERVICE_HPP

#include <string>
#include <map>

class HttpDownloadService
{
public:
  virtual ~HttpDownloadService(){}

  virtual
  std::string
  sync_get(std::string const & server,
           std::string const & path,
           std::map<std::string,std::string> const & query_params) const = 0;
};

#endif // HTTPDOWNLOADSERVICE_HPP
