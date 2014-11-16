//
// Copyright Sohail Somani (2013)
//

#ifndef INCLUDED_UNQUALIFIED_TYPE_HPP_03252013
#define INCLUDED_UNQUALIFIED_TYPE_HPP_03252013

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace di
{
  namespace traits
  {

    template<typename T>
    struct unqualified_type
    {
      typedef typename
      boost::remove_cv<
        typename boost::remove_reference<T>::type
        >::type type;
    };

  }
}

#endif // INCLUDED_UNQUALIFIED_TYPE_HPP_03252013
