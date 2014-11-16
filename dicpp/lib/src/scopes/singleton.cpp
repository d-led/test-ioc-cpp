//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/scopes/singleton.hpp"
#include "di/detail/demangle.hpp"
#include "di/logging.hpp"

std::shared_ptr<void>
di::scopes::singleton::construct( di::type_info const & type,
                                  di::type_provider & unscoped )
{
  /* TODO: The right way to do this is to have a mutex for mutating
     the state of the m_instances map and a mutex for constructing
     each type. This way, if you are trying to grab different things
     from different threads, you won't get a deadlock.

     To see what I mean: create a singleton which creates a thread in
     its constructor. The thread function accepts a provider for some
     other singleton type.

     Something like the code below should cause a deadlock:

     void thread_function(typed_provider<ASingletonType> & provider)
     {
       // deadlock here because singleton lock already grabbed to construct MyObject
       provider.get();
     }

     struct ASingletonType{};

     struct MyObject
     {
       DI_CONSTRUCTOR(MyObject,(typed_provider<ASingletonType> & provider))
       {
         new boost::thread(thread_function,std::ref(provider))->join();
       }
     }

     int main()
     {
       di::registry r;
       r.add(r.type<MyObject>().in_scope<di::scopes::singleton>());
       r.construct<MyObject&>(); // deadlock
     }

     It is worthwhile to note that Guice has/had the same problem
     (unsurprisingly):

     http://code.google.com/p/google-guice/issues/detail?id=183
  */

  // TODO: this lock should lock on the specific type, not the entire
  // data structure
  boost::recursive_mutex::scoped_lock lock(m_mutex);
  {
    instance_map_t::iterator it = m_instances.find(type);
    if(it == m_instances.end())
    {
      DI_LOG_DEBUG("Singleton: constructing: " << detail::demangle(type.name()));
      std::shared_ptr<void> ptr = unscoped.untyped_get();
      // TODO: Now, grab a lock on the entire map
      m_instances[type] = ptr;
    }
    else
    {
      DI_LOG_DEBUG("Singleton: returning existing: " << detail::demangle(type.name()));
    }
    // TODO: Grab the lock on the entire map here as well
    assert(m_instances[type]);
    return m_instances[type];

    // I can't believe 10 lines of code have this many comments
  }
}
