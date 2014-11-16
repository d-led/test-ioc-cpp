//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_DEMANGLE_HPP
#define INCLUDED_DEMANGLE_HPP

#include <string>

namespace di
{
  namespace detail
  {

    std::string
    demangle( std::string const & mangled );

  }
}

#endif // DEMANGLE_HPP
