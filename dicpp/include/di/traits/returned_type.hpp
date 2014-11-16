//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_RETURNED_TYPE_HPP
#define INCLUDED_RETURNED_TYPE_HPP

#include "di/traits/as_type_key.hpp"

namespace di
{
  namespace traits
  {

    template<typename T>
    struct returned_type
    {
      typedef typename as_type_key<T>::type type1;
      typedef typename type1::type type;
    };

  }
}


#endif // RETURNED_TYPE_HPP
