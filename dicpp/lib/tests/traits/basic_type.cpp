//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/traits/basic_type.hpp"
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/test/unit_test.hpp>

#define TEST_BASIC_TYPE(ActualType,ExpectedBasicType)                   \
  BOOST_MPL_ASSERT(( boost::is_same<                                    \
                     di::traits::basic_type<ActualType>::type,          \
                     ExpectedBasicType>))

BOOST_AUTO_TEST_CASE( TestBasicType )
{
  TEST_BASIC_TYPE(int, int);
  TEST_BASIC_TYPE(int &, int);
  TEST_BASIC_TYPE(int const &,int);
}

