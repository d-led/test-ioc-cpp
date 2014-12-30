//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_STATICSTOCKQUOTESERVICE_HPP
#define INCLUDED_STATICSTOCKQUOTESERVICE_HPP

#include "StockQuoteService.hpp"
#include "di/constructor.hpp"
#include "di/type_provider.hpp"
#include "PhoneStockQuoteService.hpp"
#include "YahooStockQuoteService.hpp"

struct StaticStockQuoteService :
  public StockQuoteService
{
  virtual
  double
  getQuote( std::string const & symbol )
  {
    return 42.0;
  }
};

struct StaticStockQuoteServiceFactory :
  public StockQuoteServiceFactory
{

  DI_CONSTRUCTOR(StaticStockQuoteServiceFactory,
                 (di::typed_provider<StaticStockQuoteService> & static_provider,
                  di::typed_provider<PhoneStockQuoteService> & phone_provider,
                  di::typed_provider<YahooStockQuoteService> & yahoo_provider))
    : m_static(&static_provider),
      m_phone(&phone_provider),
      m_yahoo(&yahoo_provider)
  {}

  virtual
  std::vector<std::string>
  availableServices() const;

  virtual
  std::shared_ptr<StockQuoteService>
  createService( std::string const & service ) const;

private:
  di::typed_provider<StaticStockQuoteService> *m_static;
  di::typed_provider<PhoneStockQuoteService> *m_phone;
  di::typed_provider<YahooStockQuoteService> *m_yahoo;

};

#endif // STATICSTOCKQUOTESERVICE_HPP
