//
// Copyright Sohail Somani (2013)
//

#include <boost/test/unit_test.hpp>
#include "di/call_function.hpp"
#include "di/registry.hpp"
#include "di/constructor.hpp"

struct AnotherType
{
  DI_CONSTRUCTOR(AnotherType,(int i))
    :m_i(i)
  {}

  int m_i;
};

struct AType
{
  DI_CONSTRUCTOR(AType,
                 (AnotherType t))
    :m_t(t)
  {}

  AnotherType m_t;
};

static
int
test_call_function(AType type)
{
  return type.m_t.m_i;
}

static
int
test_call_function2(AType a1,
                    AType a2)
{
  return a1.m_t.m_i + a2.m_t.m_i;
}

static
int test_call_function3(AType       a1,
                        AType       a2,
                        AnotherType a3)
{
  return a1.m_t.m_i + a2.m_t.m_i + a3.m_i;
}

BOOST_AUTO_TEST_CASE(test_test_call_function)
{
  di::registry r;
  r.add(
    r.type<AnotherType>()
    .instance(new AnotherType(5))
    );

  r.add(
    r.type<AType>()
    );

  BOOST_REQUIRE_EQUAL(
    di::call_function(r,test_call_function),
    5
    );

  BOOST_REQUIRE_EQUAL(
    di::call_function(r,test_call_function2),
    5*2
    );

  BOOST_REQUIRE_EQUAL(
    di::call_function(r,test_call_function3),
    5*3
    );
}
