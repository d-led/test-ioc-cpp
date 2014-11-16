//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_USERINTERFACE_HPP_05152011
#define INCLUDED_USERINTERFACE_HPP_05152011

#include "boost/noncopyable.hpp"
#include <string>

struct UserInterface
  : private boost::noncopyable
{
  virtual ~UserInterface(){}

  virtual
  void
  showMessage( std::string const & message ) = 0;

  virtual
  std::string
  getUserInput( std::string const & message ) = 0;

};

#endif // INCLUDED_USERINTERFACE_HPP_05152011
