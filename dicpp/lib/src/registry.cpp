//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/registry.hpp"
#include <functional>
#include "boost/foreach.hpp"
#include "di/scopes/eager_singleton.hpp"

using namespace di;

static
void
eagerly_initialize( registry const & r,
                    registry::construction_info const & info )
{
  if(info.scope_type == type_key<scopes::eager_singleton>())
    r.untyped_construct(info.provider->provided_type());
}

registry::registry()
{
  // bootstrap no_scope
  add_scope<scopes::no_scope>();

  m_signal_type_registered.connect(
    std::bind(eagerly_initialize,std::ref(*this),std::placeholders::_1)
    );
}

registry::~registry()
{
  BOOST_FOREACH( shutdown_func_t const & f , m_shutdown_funcs )
  {
    try{f(*this);}
    catch(...){}
  }
  // In the presence of threads, these should be destructed as a
  // logically atomic operation. Since they are not, use
  // add_shutdown_hook to stop any threads. This is a terrible
  // solution and I am sorry.
  m_providers.clear();
  m_scopes.clear();
}

std::shared_ptr<void>
registry::untyped_construct(di::type_info const & key) const
{
  DI_LOG_DEBUG("Constructing: " << detail::demangle(key.name()));

  provider_map_t::const_iterator pit = m_providers.find(key);
  if(pit == m_providers.end())
  {
    std::string message = "Requested type without corresponding provider: " +
      detail::demangle(key.name()) + ". Double check if the type is abstract";
    throw std::runtime_error(message.c_str());
  }

  std::shared_ptr<type_provider> provider = pit->second.provider;

  DI_LOG_DEBUG("Provided type: " << detail::demangle(provider->provided_type().name()));

  std::shared_ptr<void> instance = provider->untyped_get();

  DI_LOG_DEBUG("Completed constructing: " << detail::demangle(key.name())
               << " with address: " << std::hex << instance.get());

  return instance;
}
