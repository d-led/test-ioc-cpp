//
// Copyright Somani Software Construction Inc. (2011)
//

#include "di/traits/construction_scope.hpp"

#include "di/registry.hpp"
#include "di/scopes/singleton.hpp"
#include "di/scopes/thread_local_singleton.hpp"
#include <boost/test/unit_test.hpp>

template<typename T>
struct MyType
{};

struct AnotherType{};

namespace di { namespace traits {

    template<typename T>
    struct construction_scope<MyType<T> >
    {
      typedef di::scopes::thread_local_singleton type;
    };

    template<>
    struct construction_scope<MyType<int> >
    {
      typedef di::scopes::singleton type;
    };

}}

BOOST_AUTO_TEST_CASE( TestConstructionScope )
{
  di::registry r;
  r.add(
    r.type<MyType<int> >()
    );

  r.add(
    r.type<MyType<double> >()
    );

  r.add(
    r.type<AnotherType>()
    );

  BOOST_REQUIRE( di::type_info(di::type_key<di::scopes::thread_local_singleton>()) ==
                 r.construction_scope<MyType<double> >() );

  BOOST_REQUIRE( di::type_info(di::type_key<di::scopes::singleton>()) ==
                 r.construction_scope<MyType<int> > () );

  BOOST_REQUIRE( di::type_info(di::type_key<di::scopes::no_scope>()) ==
                 r.construction_scope<AnotherType> () );

}
