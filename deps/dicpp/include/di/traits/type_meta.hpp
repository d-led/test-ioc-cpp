//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_TYPE_META_HPP
#define INCLUDED_TYPE_META_HPP

#include "di/traits/constructor.hpp"
#include "di/traits/construction_scope.hpp"

namespace di
{

  namespace traits
  {

    // Do not specialize this. In fact, not sure if it should even
    // exist.
    template<typename T>
    struct type_meta
    {
      typedef typename constructor<T>::type constructor;
      typedef typename construction_scope<T>::type construction_scope;
    };
  }

}

#endif // TYPE_META_HPP
