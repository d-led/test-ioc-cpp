//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_TYPE_PROVIDER_HPP
#define INCLUDED_TYPE_PROVIDER_HPP

#include "di/type_info.hpp"
#include <memory>
#include <boost/mpl/bool.hpp>

namespace di
{

  struct type_provider
  {
    virtual
    ~type_provider();

    virtual
    di::type_info
    provided_type() const = 0;

    virtual
    std::shared_ptr<void>
    untyped_get() const = 0;
  };

}

#endif // TYPE_PROVIDER_HPP
