//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/scopes/thread_local_singleton.hpp"
#include "boost/test/unit_test.hpp"
#include "di/registry.hpp"
#include "boost/thread.hpp"
#include "boost/thread/barrier.hpp"

struct TLType
{
  static boost::thread_specific_ptr<int> m_instance;

  TLType()
  {
    if(m_instance.get())
      throw "OMG";
    m_instance.reset(new int(5));
  }
};

static
void
construct_in_thread(di::registry & r,
                    boost::barrier & b,
                    bool & test_passed)
{
  b.wait();

  test_passed = true;

  try { r.construct<TLType&>(); }
  catch(...){ test_passed = false; }

  try { r.construct<TLType&>(); }
  catch(...){ test_passed = false; }

  try{ r.construct<TLType&>(); test_passed = true; }
  catch(...){}
}

boost::thread_specific_ptr<int>
TLType::m_instance;

BOOST_AUTO_TEST_CASE(TLSingleton)
{
  di::registry r;
  r.add( r.type<TLType>().in_scope<di::scopes::thread_local_singleton>() );

  r.construct<TLType&>();
  BOOST_REQUIRE_NO_THROW(r.construct<TLType&>());
  BOOST_REQUIRE_THROW(TLType t,const char *);

  boost::barrier b(2);

  bool b1, b2;

  boost::thread
    t1(std::bind(construct_in_thread,std::ref(r),std::ref(b), std::ref(b1))),
    t2(std::bind(construct_in_thread,std::ref(r),std::ref(b), std::ref(b2)))
    ;

  t1.join();
  t2.join();

  BOOST_REQUIRE(b1 && b2);
}
