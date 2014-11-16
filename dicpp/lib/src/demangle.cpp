//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/detail/demangle.hpp"

#if defined(__GNUC__)

#include <cxxabi.h>
#include <memory>
#include <cstdlib>
#include <stdexcept>

std::string
di::detail::demangle( std::string const & mangled )
{
  char * demangled = abi::__cxa_demangle(mangled.c_str(), 0, 0, 0);
  if(!demangled)
    throw std::runtime_error("Could not demangle");
  else
  {
    // TODO: this is retarded, use a scope guard
    std::shared_ptr<char> demangled2(demangled,::free);
    return std::string(demangled);
  }
}

#else

std::string
di::detail::demangle( std::string const & mangled )
{
  return mangled;
}

#endif


