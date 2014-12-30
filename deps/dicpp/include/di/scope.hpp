//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_DI_SCOPE_HPP
#define INCLUDED_DI_SCOPE_HPP

#include "di/type_info.hpp"
#include "di/type_provider.hpp"
#include <boost/noncopyable.hpp>

namespace di
{

  struct scope :
    private boost::noncopyable
  {
    virtual
    ~scope();

    virtual
    std::shared_ptr<void>
    construct(type_info const & type,
              type_provider & unscoped) = 0;
  };

}

#endif // DI_SCOPE_HPP
