//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_BASIC_TYPE_HPP
#define INCLUDED_BASIC_TYPE_HPP

#include "di/traits/as_type_key.hpp"
#include "di/traits/remove_ptr.hpp"
#include "di/traits/unqualified_type.hpp"

namespace di
{
  namespace traits
  {

    template<typename T>
    struct basic_type
    {
      typedef typename as_type_key<T>::type::type            key;
      typedef typename unqualified_type<key>::type           unqualified;
      typedef typename remove_ptr<unqualified>::type         no_ptr;
      typedef typename boost::remove_reference<no_ptr>::type type;
    };

  }
}

#endif // BASIC_TYPE_HPP
