//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_STOCKQUOTEAPP_HPP
#define INCLUDED_STOCKQUOTEAPP_HPP

#include "di/constructor.hpp"
#include "UserInterface.hpp"
#include "StockQuoteService.hpp"

// "Controller"
struct StockQuoteApp
{
  DI_CONSTRUCTOR(StockQuoteApp,
                 (std::shared_ptr<UserInterface> ui,
                  std::shared_ptr<StockQuoteServiceFactory> factory));

  void
  execute();

private:
  void
  loop();

  std::shared_ptr<UserInterface>            m_ui;
  std::shared_ptr<StockQuoteServiceFactory> m_factory;
};

#endif // STOCKQUOTEAPP_HPP
