//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/scopes/no_scope.hpp"
#include "di/logging.hpp"
#include "di/detail/demangle.hpp"

std::shared_ptr<void>
di::scopes::no_scope::construct(di::type_info const & t,
                                di::type_provider & unscoped)
{
  DI_LOG_DEBUG("No scope constructing: " << detail::demangle(t.name()));
  return unscoped.untyped_get();
}
