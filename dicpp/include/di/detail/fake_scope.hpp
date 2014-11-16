//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_FAKE_SCOPE_HPP_05102011
#define INCLUDED_FAKE_SCOPE_HPP_05102011

#include "di/scope.hpp"

namespace di
{
  namespace detail
  {
    // This should never be used by anyone or they will be shot
    struct fake_scope : public di::scope
    {
      virtual
      std::shared_ptr<void>
      construct(type_info const & type,
                type_provider & unscoped)
      {
        assert(false);
        return std::shared_ptr<void>();
      }
    };
  }
}

#endif // INCLUDED_FAKE_SCOPE_HPP_05102011
