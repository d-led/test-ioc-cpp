//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_PROVIDED_TYPE_HPP
#define INCLUDED_PROVIDED_TYPE_HPP

#include "di/typed_provider.hpp"

namespace di
{
  namespace traits
  {

    template<typename T>
    struct provided_type;
    
    template<typename T>
    struct provided_type<typed_provider<T> >
    {
      typedef T type;
    };

  }
}

#endif // PROVIDED_TYPE_HPP
