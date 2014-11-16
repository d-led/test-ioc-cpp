//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_REGISTRATION_BUILDER_HPP
#define INCLUDED_REGISTRATION_BUILDER_HPP

#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <memory>

#include "di/traits/type_meta.hpp"
#include "di/traits/implementation.hpp"
#include "di/typed_provider.hpp"
#include "di/scope.hpp"
#include "di/scopes/no_scope.hpp"
#include "di/scopes/user_managed_scope.hpp"

namespace di
{
  struct registry;

  namespace detail
  {
    namespace tags
    {

#     define DI__BUILDER_TAG(name) struct name {}

      DI__BUILDER_TAG(annotation);
      DI__BUILDER_TAG(implementation);
      DI__BUILDER_TAG(instance);
      DI__BUILDER_TAG(in_scope);
      DI__BUILDER_TAG(constructor);
      DI__BUILDER_TAG(provider);
      // This controls if constructor parameters and child classes are
      // automatically registered or not. Defaults to true for
      // convenience.
      DI__BUILDER_TAG(register_recursive);
    }

    struct null_deleter
    {
      template<typename T>
      void
      operator()(T*)
      {}
    };

    template<
      typename T,
      typename Properties = boost::mpl::map<>
      >
    struct registration_builder
    {
    public:
      typedef Properties properties_type;

      registration_builder(){}

    private:
      template<typename T1, typename Properties1>
      friend struct registration_builder;

      template<typename T1, typename Properties1>
      registration_builder(registration_builder<T1,Properties1> const & r) :
        m_instance(r.m_instance)
      {}

    public:

      template<typename T1, typename Properties1,
               typename NewPropertyTag, typename NewProperty>
      struct new_type
      {
        typedef typename boost::mpl::insert<Properties1,
                                            boost::mpl::pair<NewPropertyTag,
                                                             NewProperty> >::type
        new_props;

        typedef registration_builder<T1,new_props> type;
      };

      template<typename T1, typename Properties1,typename Impl>
      struct new_type<T1,Properties1,tags::instance,Impl>
      {
        // When an instance is provided, the user manages the lifetime
        typedef typename boost::mpl::insert<Properties1,
                                            boost::mpl::pair<tags::in_scope,
                                                             di::scopes::user_managed_scope> >::type
        instance_props;

        typedef typename boost::mpl::insert<instance_props,
                                            boost::mpl::pair<tags::instance,
                                                             Impl> >::type
        new_props;

        typedef registration_builder<T1,new_props> type;
      };

      template<typename NewPropertyTag, typename NewProperty>
      typename new_type<T,Properties,NewPropertyTag,NewProperty>::type
      next_step()
      {
        return typename new_type<T,Properties,NewPropertyTag,NewProperty>::type(*this);
      }

      template<typename Annotation>
      typename new_type<T,Properties,tags::annotation,Annotation>::type
      annotation()
      {
        return next_step<tags::annotation,Annotation>();
      }

      template<typename Impl>
      typename new_type<T,Properties,tags::implementation,Impl>::type
      implementation()
      {
        BOOST_MPL_ASSERT((boost::is_base_of<T,Impl>));

        return next_step<tags::implementation,Impl>();
      }

      template<typename Scope>
      typename new_type<T,Properties,tags::in_scope,Scope>::type
      in_scope()
      {
        BOOST_MPL_ASSERT((boost::is_base_of<di::scope,Scope>));

        return next_step<tags::in_scope,Scope>();
      }

      template<typename ConstructorArguments>
      typename new_type<T,Properties,tags::constructor,ConstructorArguments>::type
      constructor()
      {
        return next_step<tags::constructor,ConstructorArguments>();
      }

      template<typename Impl>
      typename new_type<T,Properties,tags::instance,Impl>::type
      instance(Impl * impl)
      {
        return instance(std::shared_ptr<Impl>(impl));
      }

      template<typename Impl>
      typename new_type<T,Properties,tags::instance,Impl>::type
      instance(std::shared_ptr<Impl> const & impl)
      {
        m_instance = impl;
        // Note: check out specialization of new_type for
        // tags::instance
        return next_step<tags::instance,Impl>();
      }

      template<typename Provider>
      typename new_type<T,Properties,tags::provider,Provider>::type
      provider()
      {
        return next_step<tags::provider,Provider>();
      }

      template<bool On>
      typename new_type<T,Properties,
                        tags::register_recursive,boost::mpl::bool_<On>>::type
      register_recursive()
      {
        typedef boost::mpl::bool_<On> type;
        return next_step<tags::register_recursive,type>();
      }

    private:
      friend struct di::registry;
      std::shared_ptr<T>  m_instance;
    };

    template<typename A, typename K, typename D>
    struct at
    {
      typedef
      typename boost::mpl::has_key<A,K>::type
      has_key;

      typedef
      typename
      boost::mpl::eval_if<has_key,
                          boost::mpl::at<A,K>,
                          D>::type
      type;
    };

    namespace bm = boost::mpl;

    template<typename T, typename PropertyMap>
    struct extract_construction_properties
    {
      BOOST_MPL_ASSERT((bm::is_sequence<PropertyMap>));

      typedef T constructed_type;

      typedef
      typename at<PropertyMap,tags::annotation,bm::identity<void> >::type
      annotation;

      typedef
      typename at<PropertyMap,tags::implementation,
                  traits::implementation<T> >::type
      implementation;

      typedef
      typename at<PropertyMap,tags::in_scope,
                  traits::construction_scope<constructed_type> >::type
      scope;

      typedef
      typename at<PropertyMap,tags::constructor,
                  traits::constructor<implementation> >::type
      constructor;

      typedef
      typename at<PropertyMap,tags::provider,
                  bm::identity<typed_provider<constructed_type> > >::type
      provider;

      typedef
      typename at<PropertyMap,tags::register_recursive,
                  boost::mpl::bool_<true> >::type
      register_recursive;
    };

  }
}

#endif // REGISTRATION_BUILDER_HPP
