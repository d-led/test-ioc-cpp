//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_CONFIGITEM_HPP_05152011
#define INCLUDED_CONFIGITEM_HPP_05152011

#include "boost/signals2/signal.hpp"
#include "boost/mpl/bool.hpp"
#include "di/constructor.hpp"
#include "di/scopes/singleton.hpp"
#include "di/traits/construction_scope.hpp"
#include "ConfigurationStore.hpp"

struct ConfigItemBase
{
  virtual ~ConfigItemBase(){}

  virtual
  std::string
  itemKeyPath() const = 0;

  virtual
  std::string
  itemDescription() const = 0;

  virtual
  bool
  isReadable() const = 0;

  virtual
  std::string
  displayReadable() const = 0;

  virtual
  void
  setReadable( std::string const & s ) = 0;

};

struct AllConfigItemsBase
{
  virtual ~AllConfigItemsBase(){}

  virtual
  ConfigItemBase**
  begin() = 0;

  virtual
  ConfigItemBase**
  end() = 0;
};

template<typename T>
struct IsReadable : public boost::mpl::false_
{};

template<typename T>
struct Type{};

template<typename ValueType>
ValueType
fromReadable( std::string const &, Type<ValueType>, boost::mpl::false_ /*IsReadable*/)
{
  return ValueType();
}

template<typename ValueType>
std::string
toReadable(ValueType const  &, Type<ValueType>, boost::mpl::false_ /*IsReadable*/)
{
  return std::string("#<no-readable-representation>");
}

template<typename Tag>
struct ConfigItem :
  public ConfigItemBase
{
  /**
     Tag is a struct representing a configuration value. It consists
     of:

     ValueType: C++ type of configuration value. Must be serializable and default constructible
     keyPath(): static function returning C-string representing the key path to search in the configuration store.
     defaultValue(): static function returning default value if the store does not have the configuration value
     description(): static function returning description of configuration value
   **/

  typedef
  typename Tag::ValueType
  ValueType;

  DI_CONSTRUCTOR( ConfigItem,
                  (ConfigurationStore & store ) ):
    m_store(store),
    m_defaultValue( Tag::defaultValue() )
  {}

  virtual
  std::string
  itemKeyPath() const
  {
    return Tag::keyPath();
  }

  virtual
  std::string
  itemDescription() const
  {
    return Tag::description();
  }

  ValueType
  getValue() const
  {
    return m_store.getValue<ValueType>( Tag::keyPath(),
                                        m_defaultValue );
  }

  void
  setValue(ValueType const & V)
  {
    m_store.setValue<ValueType>( Tag::keyPath(),
                                 V );
    signalValueChanged()(V);
  }

  bool
  isReadable() const
  {
    return IsReadable<ValueType>::value;
  }

  std::string
  displayReadable() const
  {
    return toReadable(getValue(),Type<ValueType>(),IsReadable<ValueType>());
  }

  void
  setReadable( std::string const & s )
  {
    setValue(fromReadable(s,Type<ValueType>(),IsReadable<ValueType>()));
  }

  boost::signals2::signal<void(ValueType const &)> &
  signalValueChanged()
  {
    return m_signalValueChanged;
  }

private:
  ConfigurationStore & m_store;
  ValueType            m_defaultValue;

  boost::signals2::signal<void(ValueType const &)> m_signalValueChanged;
};

namespace di
{
  namespace traits
  {
    // We want all config items to be singletons
    template<typename Tag>
    struct construction_scope<ConfigItem<Tag> >
    {
      typedef di::scopes::singleton type;
    };

    template<>
    struct construction_scope<ConfigItemBase>
    {
      typedef di::scopes::singleton type;
    };
  }
}

#endif // INCLUDED_CONFIGITEM_HPP_05152011
