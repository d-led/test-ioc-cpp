//
// Copyright Somani Software Construction Inc. (2011)
//

#include "di/registry.hpp"
#include "InMemoryStore.hpp"
#include "AsioHttpServer.hpp"
#include "Application.hpp"
#include "ConsoleInterface.hpp"

void
ConfigModule(di::registry & r);

void
AppModule( di::registry & r )
{
  r.add(
    r.type<ConfigurationStore>()
    .implementation<InMemoryStore>()
    .in_scope<di::scopes::singleton>()
    );

  ConfigModule(r);

  r.add(
    r.type<HttpServer>()
    .implementation<AsioHttpServer>()
    );

  r.add(
    r.type<UserInterface>()
    .implementation<ConsoleInterface>()
    .in_scope<di::scopes::singleton>()
    );

  r.add(
    r.type<Application>()
    .in_scope<di::scopes::singleton>()
    );
}
