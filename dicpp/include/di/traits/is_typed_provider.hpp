//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_IS_TYPED_PROVIDER_HPP
#define INCLUDED_IS_TYPED_PROVIDER_HPP

#include "di/typed_provider.hpp"
#include <boost/mpl/bool.hpp>

namespace di
{
  namespace traits
  {

    template<typename T>
    struct is_typed_provider
      : public boost::mpl::false_
    {};
    
    template<typename T>
    struct is_typed_provider<typed_provider<T> > :
      public boost::mpl::true_
    {};
    
  }
}

#endif // IS_TYPED_PROVIDER_HPP
