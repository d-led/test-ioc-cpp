//
// Copyright Somani Software Construction Inc. 2010
//

/**
    Most applications using dependency injection will install a
    permutation of module functions and construct their root
    application object.

    This sample application accepts stock symbols from the user and
    retrieves them from a web service.
**/

#include "StockQuoteAppModule.hpp"
#include "StockQuoteApp.hpp"
#include "di/injector.hpp"

int main(int argc, char * argv[])
{
  di::injector inj;
  inj.install( StockQuoteAppModule() );

  StockQuoteApp & app = inj.construct<StockQuoteApp &>();
  app.execute();
}
