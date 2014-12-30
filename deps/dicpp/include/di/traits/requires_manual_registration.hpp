//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_REQUIRES_MANUAL_REGISTRATION_HPP_05222011
#define INCLUDED_REQUIRES_MANUAL_REGISTRATION_HPP_05222011

#include "boost/type_traits/is_abstract.hpp"

namespace di
{
  namespace traits
  {

    template<typename T>
    struct requires_manual_registration
      : public boost::is_abstract<T>
    {};

#define DI_REQUIRES_MANUAL_REGISTRATION(T)      \
    template<>                                  \
    struct di::traits::requires_manual_registration<T> \
      : public boost::mpl::true_                \
    {}

  }
}

#endif // INCLUDED_REQUIRES_MANUAL_REGISTRATION_HPP_05222011
