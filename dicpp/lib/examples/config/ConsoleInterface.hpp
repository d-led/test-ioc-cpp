//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_CONSOLEINTERFACE_HPP_05152011
#define INCLUDED_CONSOLEINTERFACE_HPP_05152011

#include "UserInterface.hpp"
#include <iostream>

struct ConsoleInterface :
  public UserInterface
{
  virtual
  void
  showMessage( std::string const & message )
  {
    std::cout << std::endl << "*****" << std::endl << message << std::endl << "*****" << std::endl;
  }

  virtual
  std::string
  getUserInput( std::string const & message )
  {
    std::cout << message << ": ";
    std::string result;
    std::getline(std::cin,result);
    return result;
  }

};

#endif // INCLUDED_CONSOLEINTERFACE_HPP_05152011
