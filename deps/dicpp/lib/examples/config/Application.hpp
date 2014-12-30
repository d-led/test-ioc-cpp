//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_APPLICATION_HPP_05152011
#define INCLUDED_APPLICATION_HPP_05152011

#include "di/constructor.hpp"
#include <memory>
#include "HttpServer.hpp"
#include "UserInterface.hpp"
#include "ConfigItem.hpp"

class Application
{
public:
  DI_CONSTRUCTOR(Application,
                 (std::shared_ptr<HttpServer> server,
                  std::shared_ptr<UserInterface> ui,
                  std::shared_ptr<AllConfigItemsBase> configs))
    : m_server(server),
      m_ui(ui),
      m_configs(configs)
  {}

  void
  run();

private:
  std::shared_ptr<HttpServer>         m_server;
  std::shared_ptr<UserInterface>      m_ui;
  std::shared_ptr<AllConfigItemsBase> m_configs;
};

#endif // INCLUDED_APPLICATION_HPP_05152011
