//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_NOSCOPE_HPP
#define INCLUDED_NOSCOPE_HPP

#include "di/scope.hpp"

namespace di
{
  namespace scopes
  {
    struct no_scope : public di::scope
    {
      virtual
      std::shared_ptr<void>
      construct(type_info const & type,
                type_provider & unscoped);
    };
  }
}

#endif // NOSCOPE_HPP
