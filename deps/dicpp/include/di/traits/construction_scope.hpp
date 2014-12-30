//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_CONSTRUCTION_SCOPE_HPP_05092011
#define INCLUDED_CONSTRUCTION_SCOPE_HPP_05092011

#include "di/scopes/no_scope.hpp"

namespace di
{
  namespace traits
  {

    template<typename T>
    struct construction_scope
    {
      typedef di::scopes::no_scope type;
    };

  }
}

#define DI_CONSTRUCTION_SCOPE(Type,Scope)       \
  namespace di                                  \
  {                                             \
    namespace traits                            \
    {                                           \
      template<>                                \
        struct construction_scope<Type>         \
      { typedef Scope type; };                  \
                                                \
    }                                           \
  }

#endif // INCLUDED_CONSTRUCTION_SCOPE_HPP_05092011
