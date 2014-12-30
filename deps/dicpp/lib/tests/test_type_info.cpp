//
// Copyright Somani Software Construction Inc. 2010
//

#include <boost/test/unit_test.hpp>
#include "di/type_info.hpp"

using namespace di;

BOOST_AUTO_TEST_CASE( Operators )
{
  BOOST_REQUIRE(di::type_info(type_key<int>()) != di::type_info(type_key<double>()));
  BOOST_REQUIRE(di::type_info(type_key<int>()) == di::type_info(type_key<int>()));

  di::type_info type_int = type_key<int>();
  di::type_info type_double = type_key<double>();

  BOOST_REQUIRE((type_int < type_double) || (type_double < type_int));
}

BOOST_AUTO_TEST_CASE( Name )
{
  BOOST_REQUIRE_EQUAL(di::type_info(type_key<int>()).name(),
                      typeid(type_key<int>).name());
}
