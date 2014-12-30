//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_CONFIGITEMS_HPP_05152011
#define INCLUDED_CONFIGITEMS_HPP_05152011

// Include any headers needed to parse types
#include "boost/filesystem/path.hpp"
#include "path_serialize.hpp"

#define CONFIGURATION_ITEM(Name,KeyPath,Type,DefaultValue,Description)  \
  struct Name                                                           \
  {                                                                     \
    typedef Type ValueType;                                             \
                                                                        \
    static const char * keyPath()                                       \
    { return KeyPath; }                                                 \
                                                                        \
    static ValueType defaultValue()                                     \
    { return Type(DefaultValue); }                                      \
                                                                        \
    static const char * description()                                   \
    { return Description; }                                             \
  };

#include "ConfigItems.inc"

#undef CONFIGURATION_ITEM

#include "ConfigItem.hpp"

// readablity overrides
#define DECLARE_READABLE_CONFIG_TYPE(T)                                 \
  template<>                                                            \
  struct IsReadable<T> : public boost::mpl::true_{};                    \
                                                                        \
  T                                                                     \
  fromReadable( std::string const &, Type<T>, boost::mpl::true_ );      \
                                                                        \
  std::string                                                           \
  toReadable( T const &, Type<T>, boost::mpl::true_ )

DECLARE_READABLE_CONFIG_TYPE(int);
DECLARE_READABLE_CONFIG_TYPE(boost::filesystem::path);

#endif // INCLUDED_CONFIGITEMS_HPP_05152011
