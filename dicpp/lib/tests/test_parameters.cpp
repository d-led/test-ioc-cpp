//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/traits/returned_type.hpp"
#include "di/traits/type_meta.hpp"
#include "di/registry.hpp"
#include "di/scopes/singleton.hpp"
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

struct tag{};

BOOST_AUTO_TEST_CASE( ReturnType )
{
  BOOST_MPL_ASSERT(( boost::is_same<std::shared_ptr<int>,
                     di::traits::returned_type<std::shared_ptr<int> >::type> ));

  BOOST_MPL_ASSERT(( boost::is_same<int,
                     di::traits::returned_type<int>::type >));

  // User must ensure that references are kept live by the scopes
  BOOST_MPL_ASSERT(( boost::is_same<const int &,
                     di::traits::returned_type<const int &>::type > ));

  BOOST_MPL_ASSERT(( boost::is_same<int &,
                     di::traits::returned_type<int &>::type > ));

  BOOST_MPL_ASSERT(( boost::is_same<const int &,
                     di::traits::returned_type<di::type_key<const int&,tag> >::type > ));
}

struct Parameter
{
  Parameter()
  {
    ++s_instances;
  }

  static std::size_t s_instances;
};

std::size_t Parameter::s_instances = 0;

struct Arg0{};

// Note that each ArgN takes in an instance of ArgN-1. This means that
// if you are creating an instance of Arg2, you are also creating an
// instance of Arg1 and Arg0. This ends up testing all constructor
// argument sizes of <= N, a useful result.
#define GEN(N)                                                          \
  struct BOOST_PP_CAT(Arg,N)                                            \
  {                                                                     \
    BOOST_PP_CAT(Arg,N)(                                                \
      BOOST_PP_ENUM_PARAMS(N,std::shared_ptr<Parameter> p),           \
      std::shared_ptr<BOOST_PP_CAT(Arg,BOOST_PP_DEC(N))>              \
      ){}                                                               \
  };                                                                    \
                                                                        \
  namespace di                                                          \
  {                                                                     \
    namespace traits                                                    \
    {                                                                   \
      template<>                                                        \
        struct constructor<BOOST_PP_CAT(Arg,N)>                         \
      {                                                                 \
        typedef type_key<void(                                          \
          BOOST_PP_ENUM_PARAMS(N,std::shared_ptr<Parameter> p),       \
          std::shared_ptr<BOOST_PP_CAT(Arg,BOOST_PP_DEC(N))>          \
          )>::type type;                                                \
      };                                                                \
    }                                                                   \
  }                                                                     \

#define GEN_LOOP(z,N,text) GEN(N)

BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_DEC(DI_MAX_ARGS),GEN_LOOP,~)

#define REGISTER(N) ;r.add(r.type<BOOST_PP_CAT(Arg,N)>());
#define GEN_REGISTER(z,N,text) REGISTER(N)

BOOST_AUTO_TEST_CASE( NumberOfParameters )
{
  di::registry r;
  BOOST_PP_REPEAT(BOOST_PP_DEC(DI_MAX_ARGS),GEN_REGISTER,~);

  typedef BOOST_PP_CAT(Arg,BOOST_PP_DEC(BOOST_PP_DEC(DI_MAX_ARGS))) max_args;

  std::shared_ptr<max_args> instance = r.construct<std::shared_ptr<max_args> >();
  std::size_t m = BOOST_PP_DEC(BOOST_PP_DEC(DI_MAX_ARGS));
  // See comment on GEN(N) for why this is true
  BOOST_REQUIRE_EQUAL(Parameter::s_instances,static_cast<std::size_t>((m*(m+1))/2));

}

struct TestAnnotation{};

class ClassA
{
public:
  ClassA(std::string const & s = "SOMETHING ELSE"):
    value(s)
  {
  }

  std::string value;

  typedef di::type_key<void(di::type_key<std::string,TestAnnotation>)>::type constructor;
};

BOOST_AUTO_TEST_CASE( Annotation )
{
  di::registry r;
  r.add(
    r.type<ClassA>()
    );

  r.add(
    r.type<std::string>()
    .annotation<TestAnnotation>()
    .instance(new std::string("TESTING"))
    );

  BOOST_REQUIRE_EQUAL(r.construct<ClassA>().value,"TESTING");
  // Ensure that constructing just a string (not registered) default
  // constructs the string.
  BOOST_REQUIRE_EQUAL(r.construct<std::string>(),std::string());
}
