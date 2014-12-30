//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_AS_TYPE_KEY_HPP
#define INCLUDED_AS_TYPE_KEY_HPP

#include "di/type_key.hpp"

namespace di
{
  namespace traits
  {

    template<typename T>
    struct as_type_key
    {
      typedef type_key<T> type;
    };

    template<typename T, typename A>
    struct as_type_key<type_key<T,A> >
    {
      typedef type_key<T,A> type;
    };

  }
}

#endif // AS_TYPE_KEY_HPP
