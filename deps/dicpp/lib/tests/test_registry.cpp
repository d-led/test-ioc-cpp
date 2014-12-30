//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/registry.hpp"
#include "di/scopes/singleton.hpp"
#include "di/constructor.hpp"
#include "di/traits/requires_manual_registration.hpp"
#include <iostream>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/thread/tss.hpp>

class animal
{
public:
  virtual ~animal(){}
  virtual std::string speak() = 0;
};

class cat : public animal
{
  virtual std::string speak(){ return "meow"; }
};

class dog : public animal
{
  virtual std::string speak(){ return "woof!"; }
};

class bird : public animal
{
public:
  virtual std::string speak(){ return "cheep!"; }
};

struct kennel_for_2
{
  kennel_for_2(std::shared_ptr<animal> animal1,
               std::shared_ptr<animal> animal2,
               std::shared_ptr<bird> animal3)
  {
    std::cout << "Animal 1: " << animal1->speak() << std::endl;
    std::cout << "Animal 2: " << animal2->speak() << std::endl;
    std::cout << "Bird: " << animal3->speak() << std::endl;
  }
};

struct feline{};
struct canine{};

namespace di
{
  namespace traits
  {

    template<>
    struct constructor<kennel_for_2>
    {
      typedef di::type_key<void(type_key<std::shared_ptr<animal>,feline>,
                                type_key<std::shared_ptr<animal>,canine>,
                                std::shared_ptr<bird>)>::type
      type;
    };

  }
}

struct thread_local_singleton : public di::scope
{
private:
  typedef std::map<di::type_info,std::shared_ptr<void> > instance_map_t;
  boost::thread_specific_ptr<instance_map_t> m_instances;

public:
  virtual
  std::shared_ptr<void>
  construct(di::type_info const & type,
            di::type_provider & unscoped)
  {
    if(!m_instances.get())
      m_instances.reset(new instance_map_t);
    instance_map_t::iterator it = m_instances->find(type);
    if(it == m_instances->end())
      (*m_instances)[type] = unscoped.untyped_get();
    return (*m_instances)[type];
  }
};

BOOST_AUTO_TEST_CASE(Registry)
{
  using di::registry;

  registry r;
  r.add(
    r.type<animal>()
    .annotation<feline>()
    .implementation<cat>()
    );

  r.add(
    r.type<animal>()
    .annotation<canine>()
    .implementation<dog>()
    );

  r.add(
    r.type<animal>()
    .instance(new dog)
    );

  r.add(
    r.type<kennel_for_2>()
    .in_scope<thread_local_singleton>()
    );

  BOOST_REQUIRE((std::dynamic_pointer_cast<dog>(r.construct<di::type_key<std::shared_ptr<animal> ,canine> >() )));
  BOOST_REQUIRE((std::dynamic_pointer_cast<dog>(r.construct<std::shared_ptr<animal> >())));

  r.construct<kennel_for_2 &>();
  r.construct<kennel_for_2 &>();
  r.construct<kennel_for_2 &>();
  r.construct<kennel_for_2 &>();
}

struct scope1 : public di::scopes::singleton{};
struct scope2 : public di::scopes::singleton{};

BOOST_AUTO_TEST_CASE( TestRegisteredTwice )
{
  using namespace di;

  registry r;

  r.add(
    r.type<dog>()
    .in_scope<scope1>()
    );

  r.add(
    r.type<dog>()
    .in_scope<scope2>()
    );

  BOOST_REQUIRE_EQUAL(di::type_info(di::type_key<scope2>()),
                      r.construction_scope<dog>());
}

BOOST_AUTO_TEST_CASE( TestTypeRegisteredSignal )
{
  using namespace di;
  namespace p = boost::phoenix;
  registry::construction_info info;

  registry r;
  r.signal_type_registered().connect(
    p::ref(info) = p::arg_names::arg1
    );
}

struct Base1
{

};

struct Child1
  : public Base1
{

};

struct Child2
  : public Base1 {

  Child2(int,double){}

};

struct NeedsChild2 {
  NeedsChild2(Child2*){}
};

BOOST_AUTO_TEST_CASE( TestImplementationRegistered )
{
  // Ensure that the implementation is also registerd if necessary so
  // it can be constructed independently of the Base
  di::registry r;
  r.add(
    r.type<Base1>()
    .implementation<Child1>()
    );

  BOOST_REQUIRE_NO_THROW( r.construct<Child1>() );

  // However, ensure that we can prevent automagic registration on a
  // case-by-case basis.
  r.add(
    r.type<NeedsChild2>()
    .constructor<void(Child2*)>()
    .register_recursive<false>()
    );

  // Now we are free to construct child2 as we want
  r.add(
    r.type<Child2>()
    .constructor<void(int,double)>()
    );
}

BOOST_AUTO_TEST_CASE( TestRefs )
{
  di::registry r;

  const char * s = "I like big butts and I cannot lie";

  r.add(
    r.type<std::string>()
    .instance(new std::string(s))
    .in_scope<di::scopes::no_scope>()
    );

  BOOST_REQUIRE_EQUAL(r.construct<std::string>(),
                      s);

  // can't construct an unscoped reference
  BOOST_REQUIRE_THROW(r.construct<std::string&>(),std::runtime_error);
  BOOST_REQUIRE_THROW(r.construct<std::string const&>(),std::runtime_error);

  r.add(
    r.type<std::string>()
    .instance(new std::string(s))
    .in_scope<di::scopes::singleton>()
    );

  BOOST_REQUIRE_EQUAL(r.construct<std::string>(),s);
  BOOST_REQUIRE_EQUAL(r.construct<std::string &>(),s);
  BOOST_REQUIRE_EQUAL(r.construct<std::string const &>(),s);
}

static
void
setBool( bool & b ){ b = true; }

BOOST_AUTO_TEST_CASE( TestShutdown )
{
  bool hasRun = false;
  {
    di::registry r;
    r.add_shutdown_hook( std::bind(setBool,std::ref(hasRun)));
    BOOST_REQUIRE(!hasRun);
  }
  BOOST_REQUIRE(hasRun);
}

static int nquestions = 0;

struct Question
{
  Question()
  {
    ++nquestions;
  }
};

// A third-party type that needs to be injected. Obviously, you can't
// change the constructor
struct IAmAThirdPartyTypeAMA
{
  IAmAThirdPartyTypeAMA(Question q1,
                        Question q2){}
};

// Your type that needs the third party type above
struct MySpecialType
{
  DI_CONSTRUCTOR(MySpecialType,
                 (IAmAThirdPartyTypeAMA a))
  {}
};

namespace di
{
  namespace traits
  {
    template<>
    struct requires_manual_registration<IAmAThirdPartyTypeAMA>
      : public boost::mpl::true_
    {};

    template<>
    struct constructor<IAmAThirdPartyTypeAMA>
      : public di::type_key<void(Question,Question)>
    {};
  }
}

BOOST_AUTO_TEST_CASE( TestThirdPartyThing )
{
  di::registry r;

  r.add( r.type<MySpecialType>() );
  r.add( r.type<IAmAThirdPartyTypeAMA>() );
  r.construct<MySpecialType>();
  BOOST_REQUIRE_EQUAL(nquestions, 2);
}

// test template types
template<typename Class>
struct Template {
  DI_CONSTRUCTOR_T(Template,
                   (Class c))
  {}
};

struct TemplateParameter {
  static int instances;

  TemplateParameter() {
    ++instances;
  }
};

int TemplateParameter::instances = 0;

BOOST_AUTO_TEST_CASE( TestTemplateParameter ) {
  di::registry r;

  r.add( r.type<Template<TemplateParameter>>() );
  BOOST_REQUIRE_EQUAL(TemplateParameter::instances,0);
  r.construct<Template<TemplateParameter>>();
  BOOST_REQUIRE_EQUAL(TemplateParameter::instances,1);
}
