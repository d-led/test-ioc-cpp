//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_REGISTRY_FWD_HPP
#define INCLUDED_REGISTRY_FWD_HPP

#include <map>
#include <memory>
#include <boost/signals2/signal.hpp>

#include "di/detail/registration_builder.hpp"
#include "di/scope.hpp"
#include "di/traits/returned_type.hpp"
#include "di/type_key.hpp"
#include "di/type_provider.hpp"
#include "di/factory.hpp"

namespace di
{

  struct registry
  {
    struct construction_info
    {
      construction_info(std::shared_ptr<type_provider> provider,
                        type_info scope_type):
        provider(provider),
        scope_type(scope_type)
      {}

      construction_info():
        provider(),
        scope_type()
      {}

      std::shared_ptr<type_provider> provider;
      type_info                        scope_type;
    };

    typedef std::map<di::type_info,construction_info> provider_map_t;
    typedef std::map<di::type_info,std::shared_ptr<scope> > scope_map_t;

    registry();
    ~registry();

    // Entry point into registering a type
    template<typename T>
    detail::registration_builder<T>
    type();

    // Entry point into registering a factory. Note: this doesn't
    // actually work
    template<typename T>
    detail::registration_builder<factory<T>>
    factory();

    // Add a type registration based on provided properties
    template<typename T, typename PropertyMap>
    void
    add(detail::registration_builder<T,PropertyMap> const &);

    // Add type T if it is not already added
    template<typename T, typename RegisterRecursive = boost::mpl::true_>
    void
    maybe_add();

    // Add scope T
    template<typename T>
    void
    add_scope();

    // Construct a type
    template<typename T>
    typename traits::returned_type<T>::type
    construct() const;

    template<typename T>
    std::shared_ptr<T>
    construct_ptr() const;

    // Same as construct, but untyped. Some type-safety is sacrificed
    std::shared_ptr<void>
    untyped_construct(type_info const & info) const;

    // Get construction scope for a given type or no_scope if none
    // found
    template<typename T>
    type_info
    construction_scope() const;

    // Initialize all objects constructed using the given scope.
    // Probably only makes sense for singleton scopes.
    template<typename Scope>
    std::size_t
    force_initialize();

    boost::signals2::signal<void(construction_info const &)> &
    signal_type_registered()
    { return m_signal_type_registered; }

    typedef std::function<void(di::registry&)> shutdown_func_t;

    void
    add_shutdown_hook( shutdown_func_t const & func )
    { m_shutdown_funcs.push_back(func); }

  private:
    provider_map_t               m_providers;
    scope_map_t                  m_scopes;
    boost::signals2::signal<void(construction_info const &)> m_signal_type_registered;
    std::vector<shutdown_func_t> m_shutdown_funcs;
  };

}

#endif // REGISTRY_FWD_HPP
