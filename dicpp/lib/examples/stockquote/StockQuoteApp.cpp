//
// Copyright Somani Software Construction Inc. 2010
//

#include "StockQuoteApp.hpp"
#include "StockQuoteService.hpp"

#include <sstream>
#include <functional>

StockQuoteApp::StockQuoteApp(std::shared_ptr<UserInterface> ui,
                             std::shared_ptr<StockQuoteServiceFactory> factory):
  m_ui(ui),
  m_factory(factory)
{}

void
StockQuoteApp::loop()
{
  m_ui->statusMessage("Welcome to the DI Stock Quote App. Simplifying and complicating software development since 2010.");
  std::pair<std::size_t, std::string> result = m_ui->chooseFromList("Which stock quote service would you like to use?",
                                                                    m_factory->availableServices());
  m_ui->statusMessage("You chose: " + result.second);

  std::shared_ptr<StockQuoteService> service = m_factory->createService( result.second );

  std::string symbol;
  while( (symbol = m_ui->getString("Stock symbol (type quit to quit)")) != "quit")
  {
    std::ostringstream os;
    os << "Current price for " << symbol << ": " << service->getQuote( symbol );
    m_ui->statusMessage( os.str() );
  }
}

void
StockQuoteApp::execute()
{
  m_ui->execute(std::bind(&StockQuoteApp::loop,this));
}
