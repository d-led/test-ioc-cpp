//
// Copyright Somani Software Construction Inc. (2011)
//

/**
   This example covers the use of a little hack using dependency
   injection to inject application configuration information.

   No more needing to pass around AppConfig objects or hard-coding ini
   file locations in various places or via macros. This is first-class
   automatic C++, bitches.
**/

#include "di/injector.hpp"
#include "Application.hpp"

extern void AppModule(di::registry &);

int main()
{
  di::injector inj;
  inj.install( AppModule );

  inj.construct<Application&>().run();
}

