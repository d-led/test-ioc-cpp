//
// Copyright Sohail Somani (2014)
//

#include "di/detail/registration_builder.hpp"
#include "di/registry.hpp"
#include "di/constructor.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

class Base
{
public:
  virtual ~Base(){}

  virtual
  void
  doit() const= 0;

};

struct DerivedParam{};

struct Derived : public Base {

public:
  // Constructor must be accessible
  DI_CONSTRUCTOR(Derived,(DerivedParam const &)){}

  virtual
  void
  doit() const {
  }

};

struct Derived2 : public Base {

  Derived2(int,double){}

  virtual void doit() const{}
};

#define TEST_TYPE(ActualType,ExpectedType)                      \
  BOOST_MPL_ASSERT((boost::is_same<                             \
                    ActualType,                                 \
                    ExpectedType>))

BOOST_AUTO_TEST_CASE(DerivedRegistration) {

  using namespace di;
  using namespace di::detail;

  typedef decltype(
    registration_builder<Base>()
    .implementation<Derived>()
    ) registration_type;

  typedef
    extract_construction_properties<Base,registration_type::properties_type>
    properties;

  TEST_TYPE(traits::constructor<Derived>::type,type_key<void(DerivedParam const &)>::type);
  TEST_TYPE(properties::constructed_type,Base);
  TEST_TYPE(properties::annotation,void);
  TEST_TYPE(properties::implementation,Derived);
  TEST_TYPE(properties::scope,scopes::no_scope);
  TEST_TYPE(properties::constructor,type_key<void(DerivedParam const &)>::type);

  di::registry r;

  r.add(
    r.type<Base>()
    .implementation<Derived>()
    );

}

BOOST_AUTO_TEST_CASE(ExplicitConstructor) {
  using namespace di;
  using namespace di::detail;

  typedef decltype(
    registration_builder<Base>()
    .implementation<Derived2>()
    .constructor<void(int,double)>()
    ) registration_type;

  typedef
    extract_construction_properties<Base,registration_type::properties_type>
    properties;

  // Since there is no constructor typedef, it defaults to void()
  TEST_TYPE(traits::constructor<Derived2>::type,type_key<void()>::type);
  TEST_TYPE(properties::constructed_type,Base);
  TEST_TYPE(properties::annotation,void);
  TEST_TYPE(properties::implementation,Derived2);
  TEST_TYPE(properties::scope,scopes::no_scope);
  TEST_TYPE(properties::constructor,type_key<void(int,double)>::type);

  di::registry r;

  r.add(
    r.type<Base>()
    .implementation<Derived2>()
    .constructor<void(int,double)>()
    .register_recursive<false>()
    );
}
