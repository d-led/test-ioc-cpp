//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_PHONESTOCKQUOTESERVICE_HPP
#define INCLUDED_PHONESTOCKQUOTESERVICE_HPP

#include "StockQuoteService.hpp"
#include "UserInterface.hpp"

#include "di/constructor.hpp"
#include <memory>

/**
   Here is something cool (although a bit insane): this stock quote
   service requires interaction from the user!

   Note also that this requires absolutely no gluing together by the
   developer!
 **/
struct PhoneStockQuoteService :
  public StockQuoteService
{
  DI_CONSTRUCTOR(PhoneStockQuoteService,
                 (std::shared_ptr<UserInterface> ui)) :
    m_ui(ui)
  {
  }

  virtual
  double
  getQuote( std::string const & symbol );

private:
  std::shared_ptr<UserInterface> m_ui;
};

#endif // PHONESTOCKQUOTESERVICE_HPP
