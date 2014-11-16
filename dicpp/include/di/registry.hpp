//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_REGISTRY_HPP
#define INCLUDED_REGISTRY_HPP

#define BOOST_FT_MAX_ARITY 20
#include <map>
#include <stdexcept>
#include <cassert>
#include <memory>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <functional>
#include <boost/serialization/static_warning.hpp>

#include "di/detail/demangle.hpp"
#include "di/detail/generic_provider.hpp"
#include "di/detail/registration_builder.hpp"
#include "di/registry_decl.hpp"
#include "di/scope.hpp"
#include "di/scopes/no_scope.hpp"
#include "di/traits/as_type_key.hpp"
#include "di/traits/basic_type.hpp"
#include "di/traits/is_typed_provider.hpp"
#include "di/traits/provided_type.hpp"
#include "di/traits/returned_type.hpp"
#include "di/type_info.hpp"
#include "di/type_provider.hpp"
#include "di/typed_provider.hpp"
#include "di/scoped_provider.hpp"
#include "di/logging.hpp"
#include "di/detail/fake_scope.hpp"
#include "di/traits/requires_manual_registration.hpp"

namespace di
{
  namespace detail
  {
    struct registry_scope : public scopes::no_scope{};
  }

  template<typename Scope>
  void
  registry::add_scope()
  {
    assert(m_providers.find(type_key<Scope>()) == m_providers.end());
    assert(m_scopes.find(type_key<Scope>()) == m_scopes.end());

    std::shared_ptr<Scope> new_scope(new Scope);

    m_providers.insert(
      std::make_pair(
        type_key<Scope>(),
        construction_info(
          std::shared_ptr<type_provider>(
            new detail::generic_provider_with_instance<Scope>(
              new_scope
              )
            ),
          type_key<detail::fake_scope>()
          )
        )
      );
    m_scopes[type_key<Scope>()] = new_scope;
  }

  template<typename T>
  detail::registration_builder<T>
  registry::type()
  {
    return detail::registration_builder<T>();
  }

  template<typename ConstructedType, typename Implementation, typename Constructor, typename Provider>
  std::shared_ptr<typed_provider<ConstructedType> >
  get_provider(registry&r,
               std::shared_ptr<ConstructedType> instance,
               boost::mpl::true_ /* instance provided */,
               boost::mpl::true_ /* default type provider */)
  {
    return
      std::shared_ptr<typed_provider<ConstructedType> >(
        new detail::generic_provider_with_instance<ConstructedType>(instance)
        );
  }

  template<typename ConstructedType, typename Implementation, typename Constructor, typename Provider>
  std::shared_ptr<typed_provider<ConstructedType> >
  get_provider(registry&r,
               std::shared_ptr<ConstructedType> instance,
               boost::mpl::false_ /* instance provided */,
               boost::mpl::true_ /* default type provider */)
  {
    return
      std::shared_ptr<typed_provider<ConstructedType> >(
        new detail::generic_provider<ConstructedType,Implementation,Constructor>(r)
        );
  }

  template<typename ConstructedType, typename Implementation, typename Constructor, typename Provider>
  std::shared_ptr<typed_provider<ConstructedType> >
  get_provider(registry &r,
               std::shared_ptr<ConstructedType> instance,
               boost::mpl::true_ /* instance provided */,
               boost::mpl::false_ /* default type provider */)
  {
    return
      std::shared_ptr<typed_provider<ConstructedType> >(
        new detail::generic_provider_with_instance<ConstructedType>(instance)
        );
    return std::shared_ptr<typed_provider<ConstructedType> >();
  }

  template<typename ConstructedType, typename Implementation, typename Constructor, typename Provider>
  std::shared_ptr<typed_provider<ConstructedType> >
  get_provider(registry &r,
               std::shared_ptr<ConstructedType> instance,
               boost::mpl::false_ /* instance provided */,
               boost::mpl::false_ /* default type provider */)
  {
    r.maybe_add<Provider>();
    return r.construct<std::shared_ptr<Provider> >();
  }

  namespace detail
  {

    template<typename T>
    void
    maybe_add_provided_type( registry &, boost::mpl::false_ )
    {
    }

    template<typename T>
    void
    maybe_add_provided_type( registry & r, boost::mpl::true_ )
    {
      r.maybe_add<typename traits::provided_type<T>::type>();
    }

    template<typename RegisterRecursive>
    struct add_helper;

    template<>
    struct add_helper<boost::mpl::true_>
    {

      typedef void result_type;

      template<typename A1=void, typename A2=void>
      struct result
      { typedef result_type type; };

      template<typename ArgT>
      void
      operator()(registry & r, ArgT const *) const
      {
        typedef typename traits::basic_type<ArgT>::type T;

        // If the type is a type provider, make sure that the provided type is registered
        typedef typename traits::is_typed_provider<T>::type is_provider;
        detail::maybe_add_provided_type<T>(r,is_provider());

        r.maybe_add<T>();
      }

    };

    template<>
    struct add_helper<boost::mpl::false_>
    {
      typedef void result_type;

      template<typename A1=void, typename A2=void>
      struct result
      {typedef result_type type;};

      template<typename ArgT>
      void
      operator()(registry & r, ArgT const *) const
      {}
    };

    template<typename T>
    void
    do_add(registry & r, boost::mpl::false_/* requires manual registration */)
    {
      r.add(r.type<T>());
    }

    template<typename T>
    void
    do_add(registry & r, boost::mpl::true_ /* requires manual registration */)
    {
      // We don't add such classes because we need to be told how to
      // construct them.
    }

  }

  template<typename T, typename RegisterRecursive>
  void
  registry::maybe_add()
  {
    if(m_providers.find(type_key<T>()) == m_providers.end())
    {
      typedef typename di::traits::requires_manual_registration<T> manual_registration;
      typedef boost::mpl::if_<manual_registration,manual_registration,
                              boost::mpl::bool_<!RegisterRecursive::value>> do_not_add;
      detail::do_add<T>(*this,typename do_not_add::type());
    }
  }

  template<typename T, typename PropertyMap>
  void
  registry::add(detail::registration_builder<T,PropertyMap> const & r)
  {
    typedef typename detail::extract_construction_properties<T,PropertyMap> properties;

    typedef typename properties::constructed_type   constructed_type;
    typedef typename properties::annotation         annotation;
    typedef typename properties::implementation     implementation;
    typedef typename properties::scope              construction_scope;
    typedef typename properties::constructor        constructor;
    typedef typename properties::provider           custom_provider;
    typedef typename properties::register_recursive register_recursive;

    DI_LOG_DEBUG("Registering: " <<
                 detail::demangle(typeid(constructed_type).name()) <<
                 " with implementation: " <<
                 detail::demangle(typeid(implementation).name()) <<
                 " in scope: " <<
                 detail::demangle(typeid(construction_scope).name()));

    // Recursively register parameters
    typedef typename boost::function_types::parameter_types<constructor>::type parameters;
    typedef typename boost::mpl::transform<parameters,boost::add_pointer<boost::mpl::_1> >::type pointer_parameters;
    boost::mpl::for_each<pointer_parameters>(std::bind(detail::add_helper<register_recursive>(),
                                                       std::ref(*this),
                                                       std::placeholders::_1));

    // construct the provider. The fudging around with provided
    // instance is to avoid instantiating code for abstract classes.
    typedef typename
      boost::mpl::has_key<PropertyMap,detail::tags::instance>::type
      provided_instance;

    typedef typename
      boost::is_same<typed_provider<constructed_type>,custom_provider>::type
      provided_provider;

    std::shared_ptr<typed_provider<constructed_type> > unscoped_provider(
      get_provider<constructed_type,implementation,constructor,custom_provider>(*this,r.m_instance,
                                                                                provided_instance(),
                                                                                provided_provider())
      );

    // Ensure the required scope is available
    if(m_scopes.find(type_info(type_key<construction_scope>())) == m_scopes.end())
      add_scope<construction_scope>();

    scope_map_t::iterator scope_it = m_scopes.find(type_info(type_key<construction_scope>()));

    // Construct the scoped_provider (decorator pattern)
    std::shared_ptr<typed_provider<constructed_type> > scoped_prov(
      new scoped_provider<constructed_type>(
        unscoped_provider,
        *scope_it->second
        )
      );

    /**
       Notes for the next time I look at this code and get confused:

       We need a provider for regular old objects. See registry::construct

       However, we also need a provider provider because in some
       cases, we want to delay creation of the type till some time
       later.

       So here, we create a single instance of a provider
       (scoped_prov) which will be used to construct the type itself.

       We also create a provider provider by using the scoped_prov
       instance as the provided instance. This means that the provider
       provider is nothing but a dumb pointer holder.

       Scopes and annotations do not matter for providers as far as
       their construction is concerned but annotations do matter when
       looking up the type.

       So I can bet the next time you read this, you will be even more
       confused.

       Tough luck, you shouldn't have gotten stupider.

       Sohail instance of April 2011.
     **/

    // Note: construction key requires annotation
    construction_info info(scoped_prov,type_key<construction_scope>());
    m_providers[type_key<constructed_type,annotation>()] = info;

    // Now add the provider for provider injection.
    typedef typed_provider<constructed_type> provider_type;
    typedef typed_provider<provider_type> provider_provider_type;
    std::shared_ptr<provider_provider_type> provider_provider(
      new detail::generic_provider_with_instance<provider_type>(scoped_prov)
      );

    m_providers[type_key<provider_type,annotation>()] =
      construction_info(provider_provider,
                        // use the registry scope because we are
                        // guaranteed to keep the providers around as
                        // long as the registry exists
                        type_key<detail::registry_scope>());


    // Make sure the implementation is registered so it can be
    // directly constructed if needed.
    typedef boost::is_same<implementation,constructed_type> is_same;
    typedef boost::mpl::bool_<!is_same::value && register_recursive::value> actually_add;
    detail::add_helper<actually_add>()(*this,(implementation*)0);

    // Finally, notify any listeners
    m_signal_type_registered(info);
  }

  template<typename T>
  type_info
  registry::construction_scope() const
  {
    typedef typename traits::as_type_key<T>::type T_type_key;
    provider_map_t::const_iterator it = m_providers.find(T_type_key());

    if(it == m_providers.end())
      return type_key<scopes::no_scope>();
    else
      return it->second.scope_type;
  }

  namespace detail
  {

    template<typename T>
    struct do_cast
    {
      static
      T
      doit(std::shared_ptr<T> const & v)
      {
        return *v;
      }
    };

    template<typename T>
    struct do_cast<T const &>
    {
      static
      T const &
      doit(std::shared_ptr<T> const & v)
      {
        return *v;
      }
    };

    template<typename T>
    struct do_cast<T &>
    {
      static
      T &
      doit(std::shared_ptr<T> const & v)
      {
        return *v;
      }
    };

    template<typename T>
    struct do_cast<std::shared_ptr<T> >
    {
      static
      std::shared_ptr<T>
      doit(std::shared_ptr<T> const & v)
      {
        return v;
      }
    };

    template<typename T>
    struct do_cast<std::shared_ptr<T> const &>
    {
      static
      std::shared_ptr<T>
      doit(std::shared_ptr<T> const & v)
      {
        return v;
      }
    };

  }

  // special case for shared_ptr
  namespace traits {
    template<typename T>
    struct returned_type<std::shared_ptr<T> const &> {
      typedef std::shared_ptr<T> type;
    };

    template<typename T>
    struct returned_type<std::shared_ptr<T> &> {
      typedef std::shared_ptr<T> type;
    };
  }

  template<typename T>
  std::shared_ptr<T>
  registry::construct_ptr() const
  { return construct<std::shared_ptr<T> >(); }

  template<typename T>
  typename traits::returned_type<T>::type
  registry::construct() const
  {
    typedef type_key<typename traits::basic_type<typename traits::as_type_key<T>::type>::type,
      typename traits::as_type_key<T>::type::annotation> T_type_key;

    T_type_key key;
    std::shared_ptr<void> untyped_instance = untyped_construct(key);

    typedef typename T_type_key::type provided_type;
    std::shared_ptr<provided_type> instance =
      std::static_pointer_cast<provided_type>(untyped_instance);

    // throw an exception if we are constructing a reference to an
    // unscoped object
    if(boost::is_reference<typename traits::returned_type<T>::type>::value)
    {
      // if an exception is thrown here, then make sure that the type
      // you are constructing has a non-default scope otherwise a
      // reference to a temporary would be returned which is a bad
      // idea. TODO: any way to catch this at compile time?
      provider_map_t::const_iterator pit = m_providers.find(key);
      if(pit->second.scope_type == type_key<scopes::no_scope>())
        throw std::runtime_error(("Attempted to construct a reference "
                                  "to an unscoped type: " +
                                  type_info(key).name()).c_str());
    }

    return detail::do_cast<
      typename traits::returned_type<T>::type
      >::doit(instance);
  }

  template<typename Scope>
  std::size_t
  registry::force_initialize()
  {
    std::size_t num_initialized = 0;
    type_key<Scope> scope_type;
    for(provider_map_t::const_iterator it = m_providers.begin(), end = m_providers.end();
        it != end;
        ++it)
    {
      if(it->second.scope_type == scope_type)
      {
        DI_LOG_DEBUG("force_initialize: initializing "
                     << detail::demangle(it->second.provider->provided_type().name())
                     << " for "
                     << detail::demangle(type_info(scope_type).name()));
        it->second.provider->untyped_get();
        ++num_initialized;
      }
    }
    return num_initialized;
  }

}


#endif // REGISTRY_HPP
