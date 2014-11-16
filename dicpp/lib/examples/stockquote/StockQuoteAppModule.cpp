//
// Copyright Somani Software Construction Inc. 2010
//

#include "StockQuoteAppModule.hpp"
#include "StockQuoteApp.hpp"

#include "ConsoleInterface.hpp"
#include "StaticStockQuoteService.hpp"
#include "AsioHttpDownloadService.hpp"

#include "di/scopes/singleton.hpp"

void
StockQuoteAppModule::operator()( di::registry & r ) const
{
  // In each module we define the module's root objects, in this case,
  // StockQuoteApp as well as implementations/specializations of any
  // abstract classes. For example, UserInterface is an ABC and we choose
  // the console-based UI here.

  r.add(
    r.type<StockQuoteApp>()
    .in_scope<di::scopes::singleton>()
    );

  r.add(
    r.type<UserInterface>()
    .implementation<ConsoleInterface>()
    .in_scope<di::scopes::singleton>()
    );

  r.add(
    r.type<StockQuoteServiceFactory>()
    .implementation<StaticStockQuoteServiceFactory>()
    .in_scope<di::scopes::singleton>()
    );

  r.add(
    r.type<HttpDownloadService>()
    .implementation<AsioHttpDownloadService>()
    );

  r.add(
    r.type<boost::asio::io_service>()
    .in_scope<di::scopes::singleton>()
    );
}
