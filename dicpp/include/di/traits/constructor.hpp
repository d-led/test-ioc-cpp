//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_TRAITS_CONSTRUCTOR_HPP
#define INCLUDED_TRAITS_CONSTRUCTOR_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>

#include "di/type_key.hpp"

namespace di
{

  namespace traits
  {

    BOOST_MPL_HAS_XXX_TRAIT_DEF( constructor )

    template<typename T>
    struct nested_constructor
    {
      typedef typename T::constructor type;
    };

    template<typename T>
    struct constructor
    {
      typedef
      typename boost::mpl::if_<
        has_constructor<T>,
        nested_constructor<T>,
        type_key<void()>
        >::type::type type;
    };

  }

}



#endif // TRAITS_CONSTRUCTOR_HPP
