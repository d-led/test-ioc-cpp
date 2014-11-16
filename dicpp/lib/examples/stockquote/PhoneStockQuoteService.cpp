//
// Copyright Somani Software Construction Inc. 2010
//

#include "PhoneStockQuoteService.hpp"

#include <boost/lexical_cast.hpp>

double
PhoneStockQuoteService::getQuote( std::string const & symbol )
{
  m_ui->showMessage("1) Call 1-888-STOCKSRUS\n"
                    "2) Enter the symbol on the keypad\n"
                    "3) Tell me the quoted price");
  std::string squote = m_ui->getString("Quote");
  double quote = boost::lexical_cast<double>(squote);
  return quote;
}
