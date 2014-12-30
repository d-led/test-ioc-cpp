//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_GENERIC_PROVIDER_HPP
#define INCLUDED_GENERIC_PROVIDER_HPP

#include <boost/noncopyable.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/int.hpp>
#include <type_traits>

#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#include "di/type_provider.hpp"
#include "di/logging.hpp"
#include "di/detail/demangle.hpp"
#include "di/registry_decl.hpp"

#define DI_MAX_ARGS BOOST_FT_MAX_ARITY

namespace di
{
  namespace detail
  {

    template<typename T>
    struct generic_provider_with_instance :
      public typed_provider<T>
    {
      generic_provider_with_instance(std::shared_ptr<T> i):
        m_i(i)
      {}

      virtual
      std::shared_ptr<T>
      get() const
      { return m_i; }

    private:
      std::shared_ptr<T> m_i;
    };

    template<typename Interface, typename Implementation, typename Constructor>
    struct generic_provider :
      public typed_provider<Interface>
    {
      generic_provider(registry & r)
        : m_r(&r)
      {
      }

      virtual
      std::shared_ptr<Interface>
      get() const
      {
        return construct_helper();
      }

    private:
      std::shared_ptr<Interface>
      construct_helper() const
      {
        typedef boost::mpl::int_<
        boost::function_types::function_arity<Constructor>::value
          > arity;

        DI_LOG_DEBUG("Generic constructing " <<
                     detail::demangle(typeid(Interface).name()) <<
                     " with implementation: " <<
                     detail::demangle(typeid(Implementation).name()));

        // Note: the second template parameter is unused, just used to
        // help with compile errors, if any
        return constructor_helper<Implementation,arity::value>(arity());
      }

      template<typename Impl>
      std::shared_ptr<Impl>
      constructor_helper_scalar(std::true_type /*is_scalar*/) const {
        return std::make_shared<Impl>(Impl()); // zero-initialize
      }

      template<typename Impl>
      std::shared_ptr<Impl>
      constructor_helper_scalar(std::false_type) const {
        return std::make_shared<Impl>();
      }

      template<typename Impl, int Zero>
      std::shared_ptr<Interface>
      constructor_helper(boost::mpl::int_<0>) const
      {
        // If you get a compile error here because Impl has
        // parameters, check to ensure that:
        //
        // 1) the constructor typedef and the constructor itself is
        // accessible
        //
        // 2) if you are registering Impl separately and without a
        // constructor typedef, that you have
        // recursive_register<false> when registering Interface
        return constructor_helper_scalar<Impl>(std::is_scalar<Impl>());
      }

      template<typename S, int N>
      struct at_helper
      {
        typedef typename boost::mpl::at<S,boost::mpl::int_<N> >::type type;
      };

#define DI__GENERATE_CONSTRUCTOR_ARG(N) m_r->construct<typename at_helper<parameters,N>::type>()

#define DI__GENERATE_CONSTRUCTOR_ARG1(z,N,t) DI__GENERATE_CONSTRUCTOR_ARG(N) ,

#define DI__GENERATE_CONSTRUCTOR_HELPER(N)                              \
      template<typename Impl,int ArgCount>                              \
      std::shared_ptr<Interface>                                      \
      constructor_helper(boost::mpl::int_<N>) const                     \
      {                                                                 \
        typedef typename                                                \
          boost::function_types::parameter_types<Constructor>::type parameters; \
                                                                        \
        return std::make_shared<Impl>(                                  \
            BOOST_PP_REPEAT(BOOST_PP_DEC(N),DI__GENERATE_CONSTRUCTOR_ARG1,~) \
            DI__GENERATE_CONSTRUCTOR_ARG(BOOST_PP_DEC(N))               \
          );                                                            \
      }

#define DI__GENERATE_CONSTRUCTOR_HELPER_REPEAT(z,N,t)   \
      DI__GENERATE_CONSTRUCTOR_HELPER(N)

      BOOST_PP_REPEAT_FROM_TO(1,DI_MAX_ARGS,DI__GENERATE_CONSTRUCTOR_HELPER_REPEAT,~)

    private:
      registry *m_r;
    };

  }
}

#endif // GENERIC_PROVIDER_HPP
