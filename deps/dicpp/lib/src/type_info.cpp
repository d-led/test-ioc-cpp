//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/type_info.hpp"
#include "di/detail/demangle.hpp"
#include <ostream>

bool
di::type_info::operator<(type_info const & rhs) const
{ return m_info->before(*rhs.m_info) ? true : false; }

bool
di::type_info::operator==(type_info const & rhs) const
{ return !(*this < rhs) && !(rhs < *this); }

bool
di::type_info::operator!=(type_info const & rhs) const
{ return !(*this == rhs); }

std::string
di::type_info::name() const
{ return m_info->name(); }

di::type_info::type_info() :
  m_info(&typeid(di::type_key<void>))
{}

std::ostream&
di::operator<<(std::ostream & os, const type_info & info)
{
  return os << detail::demangle(info.name());
}
