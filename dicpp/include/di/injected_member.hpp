//
// Copyright Sohail Somani (2014)
//

#ifndef INCLUDED_INJECTED_MEMBER_HPP_02272014
#define INCLUDED_INJECTED_MEMBER_HPP_02272014

#include "di/injector.hpp"
#include <memory>

/**
   Some types and functions to help injecting into types that cannot
   be constructed via dicpp. Usage:

   struct InjectedType
   {

     // Note: constructor requires Something and SomethingElse ->
     // cannot be default constructedo
     DI_CONSTRUCTOR(InjectedType,(Something,SomethingElse));
   };

   struct ConstructedBySomethingElse
   {
     di::injected_member<InjectedType> m_type;

     ConstructedBySomethingElse()
     {
       InjectedType type = m_type.get();
     }
   };

   int main()
   {
     di::injector inj(di::injector::global);
     inj.install(whatever)

     ThirdPartyEventLoop t;

     // now, whenever ConstructedBySomethingElse is constructed by the
     // third party library, the injected type will be automatically
     // constructed as well.

     t.exec();
   }

   The thread-safety of this is a little iffy, I haven't fully thought
   it out, to be honest but my gut instinct is that it is as
   thread-safe as the scopes that construct the types and so should be
   fine.

   Ensure that the injector passed in to set_global_injector remains
   alive until the application exits.
 **/

namespace di
{
  template<typename T>
  struct injected_member
  {
    T m_t;

    injected_member();

    T const & get() const
    { return m_t; }

    T & get()
    { return m_t; }
  };

  template<typename T>
  struct injected_ptr :
    public injected_member<std::shared_ptr<T>>
  {
    typedef injected_member<std::shared_ptr<T>> base_type;

    T * operator->() const
    { return base_type::get().get(); }

    T & operator*() const
    { return *base_type::get().get(); }
  };

  template<typename T>
  struct injected_provider_ref :
    public injected_member<typed_provider<T>&>
  {
    typedef injected_member<typed_provider<T>&> base_type;

    T get_value()
    { return *base_type::get().get(); }
  };

  namespace detail
  {
    void
    set_global_injector( injector & inj );

    injector &
    get_global_injector();

    template<typename T>
    T
    inject_from_global()
    {
      return get_global_injector().construct<T>();
    }
  }

  template<typename T>
  injected_member<T>::injected_member()
    : m_t(detail::inject_from_global<T>())
  {}
}

#endif // INCLUDED_INJECTED_MEMBER_HPP_02272014
