//
// Copyright Sohail Somani (2014)
//

#include "di/injected_member.hpp"

using namespace di;

static injector *g_inj = 0;

void
di::detail::set_global_injector( injector & inj )
{ g_inj = &inj; }

injector &
di::detail::get_global_injector()
{
  assert(g_inj && "Must have a global injector before using injected_member");
  return *g_inj;
}
