//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/scopes/thread_local_singleton.hpp"
#include "di/logging.hpp"

std::shared_ptr<void>
di::scopes::thread_local_singleton::construct(di::type_info const & type,
                                              di::type_provider & unscoped)
{
  if(!m_instance.get())
    m_instance.reset(new singleton);
  DI_LOG_DEBUG("Thread-local singleton construct");
  return m_instance->construct(type,unscoped);
}
