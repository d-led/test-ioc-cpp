//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_TYPED_PROVIDER_HPP
#define INCLUDED_TYPED_PROVIDER_HPP

#include "di/type_provider.hpp"
#include "di/type_info.hpp"
#include "di/type_key.hpp"
#include <memory>

namespace di
{

  template<typename T>
  struct typed_provider :
    public type_provider
  {
    virtual
    di::type_info
    provided_type() const
    { return type_key<T>(); }

    virtual
    std::shared_ptr<T>
    get() const = 0;

    virtual
    std::shared_ptr<void>
    untyped_get() const
    {
      return get();
    }
  };

}

#endif // TYPED_PROVIDER_HPP
