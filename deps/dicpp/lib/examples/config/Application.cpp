//
// Copyright Somani Software Construction Inc. (2011)
//

#include "Application.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"
#include <vector>

// This function is a mess but it's not the point so :P
void
Application::run()
{
  m_ui->showMessage("Welcome to Dr. Strangelove's HTTP server.");
  std::string input;
  while(true)
  {
    input = m_ui->getUserInput("What is thy bidding, my master?");
    if(input == "quit")
    {
      m_ui->showMessage("Stopping HTTP server");
      m_server->stop();
      break;
    }
    else if(input == "stop")
    {
      m_ui->showMessage("Stopping HTTP server");
      m_server->stop();
    }
    else if(input == "start")
    {
      m_ui->showMessage("Starting HTTP server");
      m_server->start();
    }
    else if(input.find("config") != std::string::npos)
    {
      std::vector<std::string> params;
      boost::algorithm::split(params,input,boost::algorithm::is_any_of(" "));
      if(params.size() == 1)
      {
        // just list all config items and values
        for(ConfigItemBase** it = m_configs->begin(), **end = m_configs->end();
            it != end;
            ++it)
        {
          std::ostringstream os;
          os << (*it)->itemKeyPath() << " " << (*it)->itemDescription() << ". ";
          os << "Current value: " << (*it)->displayReadable();
          m_ui->showMessage(os.str());
        }
      }
      else if(params.size() >= 3)
      {
        if(params[1] == "set" || params[1] == "get")
        {
          // config set <key> <value>
          ConfigItemBase *item = 0;
          for(ConfigItemBase** it = m_configs->begin(), **end = m_configs->end();
              it != end;
              ++it)
          {
            if((*it)->itemKeyPath() == params[2])
            {
              item = *it;
              break;
            }
          }
          if(item == 0)
          {
            m_ui->showMessage("Configuration value with given keypath not found");
            continue;
          }
          else if (params[1] == "set" && params.size() == 4)
            item->setReadable( params[3] );
          std::ostringstream os;
          os << "Config item " << item->itemKeyPath() << " has value " << item->displayReadable();
          m_ui->showMessage(os.str());
        }
        else
        {
          m_ui->showMessage("Unknown config command");
          continue;
        }

      }
    }
    else
    {
      m_ui->showMessage("Command: start, stop, config, quit");
    }
  }
  m_ui->showMessage("Thank you for using Dr. Strangelove's HTTP server.");
}

