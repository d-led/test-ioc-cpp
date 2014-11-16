//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_ALLCONFIGURATIONITEMS_HPP_05162011
#define INCLUDED_ALLCONFIGURATIONITEMS_HPP_05162011

#include "ConfigItems.hpp"
#include "di/traits/constructor.hpp"

struct Dummy{};

/**
   We jump through these hoops to ensure that we have only a single
   ConfigItem<Tag> instance.
 **/
struct AllConfigurationItems
  : public AllConfigItemsBase
{

#define CONFIGURATION_ITEM(Name,KP,T,DV,D) ConfigItem<Name> &,
  AllConfigurationItems(
    #include "ConfigItems.inc"
    Dummy
    );
#undef CONFIGURATION_ITEM

  ConfigItemBase**
  begin()
  { return &m_configItems[0]; }

  ConfigItemBase**
  end()
  { return &m_configItems[0] + m_configItems.size(); }

private:
  std::vector<ConfigItemBase*> m_configItems;
};

namespace di
{
  namespace traits
  {
    template<>
    struct constructor<AllConfigurationItems>
    {
#define CONFIGURATION_ITEM(Name,KP,T,DV,D) ConfigItem<Name> &,
      typedef type_key<void(
#include "ConfigItems.inc"
        Dummy
        )>::type type;
#undef CONFIGURATION_ITEM
    };
  }
}

#endif // INCLUDED_ALLCONFIGURATIONITEMS_HPP_05162011
