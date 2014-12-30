//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_STOCKQUOTESERVICE_HPP
#define INCLUDED_STOCKQUOTESERVICE_HPP

#include <string>
#include <vector>
#include <memory>

struct StockQuoteService
{
  virtual ~StockQuoteService(){}

  virtual
  double
  getQuote( std::string const & symbol ) = 0;
};

struct StockQuoteServiceFactory
{
  virtual ~StockQuoteServiceFactory(){}

  virtual
  std::vector<std::string>
  availableServices() const = 0;
  
  virtual
  std::shared_ptr<StockQuoteService>
  createService( std::string const & service ) const = 0;
};

#endif // STOCKQUOTESERVICE_HPP
