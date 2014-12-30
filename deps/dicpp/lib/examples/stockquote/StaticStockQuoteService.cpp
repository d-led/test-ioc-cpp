//
// Copyright Somani Software Construction Inc. 2010
//

#include "StaticStockQuoteService.hpp"
#include "PhoneStockQuoteService.hpp"

#include <stdexcept>

std::vector<std::string>
StaticStockQuoteServiceFactory::availableServices() const
{
    std::vector<std::string> services;
    services.push_back("static");
    services.push_back("phone");
    services.push_back("yahoo");
    return services;  
}

std::shared_ptr<StockQuoteService>
StaticStockQuoteServiceFactory::createService( std::string const & service ) const
{
  if (service == "static" )
    return m_static->get();
  else if (service == "phone" )
    return m_phone->get();
  else if (service == "yahoo")
    return m_yahoo->get();
  else
    throw std::runtime_error("Invalid service");
}
