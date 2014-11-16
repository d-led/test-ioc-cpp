//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_INJECTOR_HPP
#define INCLUDED_INJECTOR_HPP

#include <boost/function.hpp>
#include "di/registry.hpp"
#include "di/traits/returned_type.hpp"

namespace di
{

  struct injector
  {
  private:
    registry m_r;

  public:
    enum locality
    {
      local,
      global
    };

    explicit injector(locality l = local);

    injector&
    install(std::function<void(registry & r)> const & register_types)
    {
      register_types(m_r);
      return *this;
    }

    template<typename T>
    typename traits::returned_type<T>::type
    construct() const
    {
      return m_r.construct<T>();
    }

    template<typename T>
    std::shared_ptr<T>
    construct_ptr() const
    {
      return m_r.construct<std::shared_ptr<T> >();
    }

    registry const &
    get_registry() const{ return m_r; }

  };

}

#endif // INJECTOR_HPP
