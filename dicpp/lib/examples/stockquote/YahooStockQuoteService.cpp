//
// Copyright Somani Software Construction Inc. 2010
//

#include "YahooStockQuoteService.hpp"
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>

YahooStockQuoteService::YahooStockQuoteService( di::typed_provider<HttpDownloadService> & http ):
  m_http(http)
{}

double
YahooStockQuoteService::getQuote( std::string const & symbol )
{
  using namespace boost::assign;
  std::string result = m_http.get()->sync_get("download.finance.yahoo.com",
                                              "/d/quotes.csv",
                                              map_list_of("s",symbol)("f","l1"));
  result = result.substr(0,result.size()-2); // remove trailing \r\n

  return boost::lexical_cast<double>(result);
                                                            
}
