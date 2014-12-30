//
// Copyright Sohail Somani (2013)
//

#ifndef INCLUDED_USER_MANAGED_SCOPE_HPP_05172013
#define INCLUDED_USER_MANAGED_SCOPE_HPP_05172013

#include "di/scopes/no_scope.hpp"

namespace di
{
  namespace scopes
  {
    struct user_managed_scope : public di::scopes::no_scope
    {};
  }
}

#endif // INCLUDED_USER_MANAGED_SCOPE_HPP_05172013
