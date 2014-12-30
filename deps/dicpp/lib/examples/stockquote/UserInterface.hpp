//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_USERINTERFACE_HPP
#define INCLUDED_USERINTERFACE_HPP

#include <string>
#include <vector>
#include <boost/function.hpp>

struct UserInterface
{
  virtual ~UserInterface(){}

  virtual
  void
  showMessage( std::string const & msg ) = 0;

  virtual
  std::pair<std::size_t, std::string>
  chooseFromList( std::string const & msg,
                  std::vector<std::string> const & choices ) = 0;

  virtual
  void
  statusMessage( std::string const & msg ) = 0;

  virtual
  std::string
  getString( std::string const & msg ) = 0;

  // Execute main loop code. Defaults to just plain running function.
  virtual
  void
  execute(std::function<void()> const &) ;
  
};

#endif // USERINTERFACE_HPP
