//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_INMEMORYSTORE_HPP_05152011
#define INCLUDED_INMEMORYSTORE_HPP_05152011

#include "ConfigurationStore.hpp"
#include <map>

// Only stores in memory
struct InMemoryStore :
  public ConfigurationStore
{
  virtual
  void
  setValue( const char * K, std::string const & V )
  {
    m_values[K] = V;
  }

  virtual
  bool
  hasKey( const char * K ) const
  {
    return m_values.find(K) != m_values.end();
  }

  virtual
  std::string
  getValue( const char * K ) const
  {
    std::map<std::string,std::string>::const_iterator it = m_values.find(K);
    if(it == m_values.end())
      return std::string();
    else
      return it->second;
  }

private:
  std::map<std::string,std::string> m_values;
};

#endif // INCLUDED_INMEMORYSTORE_HPP_05152011
