//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_TYPE_KEY_HPP
#define INCLUDED_TYPE_KEY_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace di
{
  template<typename BasicType, typename Annotation = void>
  struct type_key
  {
    typedef BasicType  type;
    typedef Annotation annotation;

    BOOST_MPL_ASSERT(( boost::is_same<typename boost::remove_cv<BasicType>::type,
                       BasicType> ) );
  };
}

#endif // TYPE_KEY_HPP
