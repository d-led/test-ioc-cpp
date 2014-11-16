//
// Copyright Sohail Somani (2013)
//

#ifndef INCLUDED_CALL_FUNCTION_HPP_05312013
#define INCLUDED_CALL_FUNCTION_HPP_05312013

#include <boost/function_types/result_type.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include "di/registry.hpp"

namespace di
{

#define DI__GENERATE_CF_ARG(z,N,data) r.construct<BOOST_PP_CAT(T,N)>()

#define DI__GENERATE_CF_HELPER(z,N,data)                        \
  template<typename R, BOOST_PP_ENUM_PARAMS(N,typename T)>      \
  R                                                             \
  call_function_helper(di::registry const & r,                  \
                       R(*f)(BOOST_PP_ENUM_PARAMS(N,T)))        \
  {                                                             \
    return f(BOOST_PP_ENUM(N,DI__GENERATE_CF_ARG,~));           \
  }

  BOOST_PP_REPEAT_FROM_TO(1,
                          BOOST_PP_DEC(DI_MAX_ARGS),
                          DI__GENERATE_CF_HELPER,~)

  template<typename FunctionType>
  typename boost::function_types::result_type<FunctionType>::type
  call_function(di::registry const & r,
                FunctionType const & f)
  {
    return call_function_helper(r,f);
  }
}

#endif // INCLUDED_CALL_FUNCTION_HPP_05312013
