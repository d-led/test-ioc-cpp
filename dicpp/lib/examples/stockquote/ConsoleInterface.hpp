//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_CONSOLEINTERFACE_HPP
#define INCLUDED_CONSOLEINTERFACE_HPP

#include "UserInterface.hpp"
#include <iostream>

struct ConsoleInterface :
  public UserInterface
{

  virtual
  void
  showMessage( std::string const & msg )
  {
    std::cout << msg << std::endl << "Press enter to continue" << std::endl;
    std::cin.get();
  }

  virtual
  std::pair<std::size_t, std::string>
  chooseFromList( std::string const & msg,
                  std::vector<std::string> const & choices )
  {
    std::cout << msg << std::endl;
    for(std::size_t ii = 0; ii < choices.size(); ++ii)
      std::cout << ii+1 << ": " << choices[ii] << std::endl;

    std::size_t idx;
    std::cout << "Enter your choice (1-" << choices.size() << ") and press enter: ";
    std::cin >> idx;
    std::cin.get(); // enter

    if (idx <= choices.size())
      return std::make_pair(idx-1, choices[idx-1]);
    else
      throw std::runtime_error("Invalid choice");
  }

  virtual
  void
  statusMessage( std::string const & msg )
  {
    std::cout << msg << std::endl;
  }

  virtual
  std::string
  getString( std::string const & msg )
  {
    std::cout << msg << ": ";
    std::string result;
    std::getline(std::cin,result);
    return result;
  }
};

#endif // CONSOLEINTERFACE_HPP
