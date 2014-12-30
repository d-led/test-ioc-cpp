//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_CONFIGURATIONSTORE_HPP_05152011
#define INCLUDED_CONFIGURATIONSTORE_HPP_05152011

#include <string>
#include <sstream>
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

struct ConfigurationStore
{
  virtual ~ConfigurationStore(){}

  virtual void setValue( const char *K, std::string const & V) = 0;
  virtual bool hasKey( const char * K ) const                = 0;
  virtual std::string getValue( const char * K ) const         = 0;

  template<typename Value>
  Value
  getValue( const char * K, const Value & defaultValue ) const
  {
    if(!hasKey(K))
      return defaultValue;

    Value ret;
    std::istringstream is(getValue(K));
    {
      boost::archive::text_iarchive ia(is);
      ia >> ret;
    }
    return ret;
  }

  template<typename Value>
  void
  setValue( const char * K, const Value & V )
  {
    std::ostringstream os;
    {
      boost::archive::text_oarchive oa(os);
      oa << V;
    }
    setValue(K,os.str());
  }

};

#endif // INCLUDED_CONFIGURATIONSTORE_HPP_05152011
