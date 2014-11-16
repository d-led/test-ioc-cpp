//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_TYPE_INFO_HPP
#define INCLUDED_TYPE_INFO_HPP

#include <typeinfo>
#include <string>
#include <iosfwd>
#include "di/type_key.hpp"

namespace di
{
  // Note: Not portable!
  struct type_info
  {
    template<typename T,typename A>
    /*implicit*/
    type_info(type_key<T,A>);

    type_info();

    bool
    operator<(type_info const & rhs) const;

    bool
    operator==(type_info const & rhs) const;

    bool
    operator!=(type_info const & rhs) const;

    std::string
    name() const;

  private:
    std::type_info const *m_info;
  };

  template<typename T, typename A>
  type_info::type_info(type_key<T,A>):
    m_info(&typeid(type_key<T,A>))
  {}

  std::ostream&
  operator<<(std::ostream & os, const type_info &);
}

#endif // TYPE_INFO_HPP
