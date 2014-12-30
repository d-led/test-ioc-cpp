//
// Copyright Somani Software Construction Inc. (2011)
//

#include "di/injector.hpp"
#include "di/injected_member.hpp"

using namespace di;

injector::injector( locality l)
{
  if(l == global)
    detail::set_global_injector(*this);
}

