//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/registry.hpp"
#include "di/scopes/singleton.hpp"
#include "di/scopes/eager_singleton.hpp"
#include "di/injector.hpp"
#include <boost/test/unit_test.hpp>
#include <iostream>

struct is_singleton : public di::scope
{
public:
  std::size_t nobjects;

  static std::size_t ninstances;
  is_singleton():
    nobjects(0)
  {
    ++ninstances;
  }

  virtual
  std::shared_ptr<void>
  construct(di::type_info const & type,
            di::type_provider & unscoped)
  {
    std::cout << di::detail::demangle(type.name()) << std::endl;
    ++nobjects;
    return unscoped.untyped_get();
  }
};

std::size_t
is_singleton::ninstances = 0;

BOOST_AUTO_TEST_CASE( TestIsOnlyConstructedOnce )
{
  di::registry r;
  r.add( r.type<int>().instance(new int(5)).in_scope<is_singleton>() );
  r.add( r.type<double>().instance( new double(10) ).in_scope<is_singleton>() );

  for(int ii = 0; ii < 3; ++ii)
  {
    BOOST_REQUIRE_EQUAL(r.construct<int>(), 5);
    BOOST_REQUIRE_EQUAL(r.construct<double>(), 10);
  }

  BOOST_REQUIRE_EQUAL(is_singleton::ninstances,1U);
  // This next comparison used to require 6 objects + 1 for scope but
  // for the life of me, I cannot remember why 7 objects were expected
  // to have been constructed.... I am guessing that it must be
  // because scopes are no longer scoped... which sounds wrong,
  // therefore this prior test was correct and the new behaviour is
  // incorrect. Actually, thinking about it a bit further, the
  // previous test seems to be incorrect because the scope should have
  // already been constructed, therefore the r.construct<> call for
  // is_singleton should *not* have constructed another type. In fact,
  // I am now convinced that the previous behaviour was *incorrect* In
  // any case, I'm leaving the above comment in case my reasoning for
  // altering the test turns out to be incorrect so I can read it in
  // the future and be confused again.
  BOOST_REQUIRE_EQUAL(r.construct<is_singleton&>().nobjects,6U);
  BOOST_REQUIRE_EQUAL(is_singleton::ninstances,1U); // still should only be constructed once
}

static std::size_t obj_instances = 0;

struct AnObject :
  private boost::noncopyable
{
  AnObject()
  {
    ++obj_instances;
  }

};

BOOST_AUTO_TEST_CASE( TestSingletonScope )
{
  di::registry r;
  r.add( r.type<AnObject>().in_scope<di::scopes::singleton>() );

  BOOST_REQUIRE_EQUAL(obj_instances,0U);
  r.construct<AnObject&>();
  BOOST_REQUIRE_EQUAL(obj_instances,1U);

  r.construct<AnObject&>();
  BOOST_REQUIRE_EQUAL(obj_instances,1U);
}

// Previously, providers were unscoped, making provider injection
// largely useless
BOOST_AUTO_TEST_CASE( ScopedProvider )
{
  di::registry r;
  r.add( r.type<AnObject>().in_scope<di::scopes::singleton>() );

  obj_instances = 0;
  BOOST_REQUIRE_EQUAL(obj_instances,0U);

  std::shared_ptr<di::typed_provider<AnObject> > provider =
    r.construct<std::shared_ptr<di::typed_provider<AnObject> > >();

  // Ensure that the typed_provider is held by something else
  BOOST_REQUIRE( !provider.unique() );

  // Ensure no instances
  BOOST_REQUIRE_EQUAL(obj_instances,0U);

  // Get the instance
  std::shared_ptr<AnObject> object = provider->get();

  // Should be one instance
  BOOST_REQUIRE_EQUAL ( obj_instances, 1U);

  std::shared_ptr<AnObject> object2 = provider->get();

  // Here's the bug. When providers were unscoped, every get was a new
  // instance. Applications should not need unscoped providers unless
  // they are writing scopes.
  BOOST_REQUIRE_EQUAL( obj_instances, 1U);
}

struct ensure_destructed : public di::scope
{
  static bool constructed, destructed;

  ensure_destructed()
  {
    constructed = true;
    destructed = false;
  }

  ~ensure_destructed()
  {
    destructed = true;
  }

  virtual
  std::shared_ptr<void>
  construct(di::type_info const & type,
            di::type_provider & unscoped)
  {
    return unscoped.untyped_get();
  }
};

bool
ensure_destructed::constructed = false;

bool
ensure_destructed::destructed = false;

BOOST_AUTO_TEST_CASE( ProviderDestruction )
{
  {
    di::registry r;

    {
      r.add( r.type<AnObject>().in_scope<ensure_destructed>() );

      obj_instances = 0;

      std::shared_ptr<di::typed_provider<AnObject> > provider =
        r.construct<std::shared_ptr<di::typed_provider<AnObject> > >();

      BOOST_REQUIRE(ensure_destructed::constructed);
      BOOST_REQUIRE(!ensure_destructed::destructed);
    }

    // After the registry is destroyed, scopes should also be
    // destroyed.
    BOOST_REQUIRE(ensure_destructed::constructed);
    BOOST_REQUIRE(!ensure_destructed::destructed);

  } // Destroy registry

  BOOST_REQUIRE(ensure_destructed::constructed);
  BOOST_REQUIRE(ensure_destructed::destructed);
}

static int InitializeCount = 0;

struct MyType
{
  MyType()
  {
    ++InitializeCount;
  }
};

BOOST_AUTO_TEST_CASE( TestEagerSingleton )
{
  di::registry r;
  BOOST_REQUIRE_EQUAL( InitializeCount, 0);

  r.add(
    r.type<MyType>()
    .in_scope<di::scopes::eager_singleton>()
    );

  BOOST_REQUIRE_EQUAL( InitializeCount, 1);

  // Lazy way to check that re-registering type does not cause multiple initialization
  r.add(
    r.type<MyType>()
    .in_scope<di::scopes::eager_singleton>()
    );

  BOOST_REQUIRE_EQUAL( InitializeCount, 1);
}

BOOST_AUTO_TEST_CASE( InstanceHasUserScope )
{
  di::registry r;

  r.add(
    r.type<int>()
    .instance(new int(5))
    );

  r.add(
    r.type<double>()
    );

  BOOST_REQUIRE_EQUAL( r.construction_scope<double>(),
                       di::type_key<di::scopes::no_scope>());

  BOOST_REQUIRE_EQUAL( r.construction_scope<int>(),
                       di::type_key<di::scopes::user_managed_scope>() );
}
