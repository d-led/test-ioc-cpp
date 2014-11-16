//
// Copyright Sohail Somani (2014)
//

#include "di/constructor.hpp"
#include "di/injected_member.hpp"
#include "di/scopes/singleton.hpp"
#include <boost/test/unit_test.hpp>

using namespace di;

int g_param_count = 0;

struct Param
{
  Param()
  {
    ++g_param_count;
  }
};

struct InjectedMember
{
  DI_CONSTRUCTOR(InjectedMember,
                 (Param,Param,Param))
  {}
};

struct ManuallyConstructedType
{
  di::injected_member<InjectedMember> injected;
};

void TestModule( registry & r )
{
  r.add(r.type<InjectedMember>());
}

BOOST_AUTO_TEST_CASE(test_injected_member)
{
  g_param_count = 0;
  injector inj(injector::global);
  inj.install(TestModule);

  BOOST_REQUIRE(g_param_count == 0);
  ManuallyConstructedType t;
  BOOST_REQUIRE(g_param_count == 3);
  ManuallyConstructedType t2;
  BOOST_REQUIRE(g_param_count == 6);
}

// same test as before but this time, it is a singleton type that is
// injected as a member
void TestModule2( registry & r )
{
  r.add(r.type<InjectedMember>()
        .in_scope<scopes::singleton>());
}

BOOST_AUTO_TEST_CASE(test_injected_member2)
{
  g_param_count = 0;
  injector inj(injector::global);
  inj.install(TestModule2);

  BOOST_REQUIRE(g_param_count == 0);
  ManuallyConstructedType t;
  BOOST_REQUIRE(g_param_count == 3);
  ManuallyConstructedType t2;
  // Note that even though the type is injected manually, it is only
  // constructed once and copy constructed the remainder of times.
  BOOST_REQUIRE(g_param_count == 3);
}

struct ManuallyConstructedType2
{
  di::injected_ptr<InjectedMember> injected;
};

void TestModule3( registry & r )
{
  r.add(r.type<InjectedMember>());
}

BOOST_AUTO_TEST_CASE(test_injected_ptr)
{
  g_param_count = 0;
  injector inj(injector::global);
  inj.install(TestModule3);

  BOOST_REQUIRE(g_param_count == 0);
  ManuallyConstructedType2 t;
  BOOST_REQUIRE(g_param_count == 3);
  std::shared_ptr<InjectedMember> ptr = t.injected.get();
  BOOST_REQUIRE(ptr.use_count() == 2);
}

// test injected provider
struct ManuallyConstructedType3
{
  di::injected_provider_ref<InjectedMember> provider;
};

BOOST_AUTO_TEST_CASE(test_injected_provider_ref)
{
  g_param_count = 0;
  injector inj(injector::global);
  inj.install(TestModule3);

  BOOST_REQUIRE(g_param_count == 0);
  ManuallyConstructedType3 t3;
  // NOTE: this should be 0 because we have only injected the
  // provider, not the value itself
  BOOST_REQUIRE(g_param_count == 0);
  // Now request the actual value
  t3.provider.get_value();
  BOOST_REQUIRE(g_param_count == 3);
  t3.provider.get_value();
  BOOST_REQUIRE(g_param_count == 6);
}
