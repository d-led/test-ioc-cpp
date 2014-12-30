//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_YAHOOSTOCKQUOTESERVICE_HPP
#define INCLUDED_YAHOOSTOCKQUOTESERVICE_HPP

#include "StockQuoteService.hpp"
#include "HttpDownloadService.hpp"
#include "di/constructor.hpp"
#include "di/typed_provider.hpp"

struct YahooStockQuoteService : 
  public StockQuoteService
{
  DI_CONSTRUCTOR(YahooStockQuoteService,
                 (di::typed_provider<HttpDownloadService> & http));
  
  virtual
  double
  getQuote( std::string const & symbol );

private:
  di::typed_provider<HttpDownloadService> &m_http;
};


#endif // YAHOOSTOCKQUOTESERVICE_HPP
