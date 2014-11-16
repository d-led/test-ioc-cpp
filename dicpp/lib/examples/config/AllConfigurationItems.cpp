//
// Copyright Somani Software Construction Inc. (2011)
//

#include "AllConfigurationItems.hpp"
#include "boost/preprocessor/cat.hpp"
#include "di/scopes/singleton.hpp"
#include "di/registry.hpp"
#include "boost/lexical_cast.hpp"

#define CONFIGURATION_ITEM(Name,KP,T,DV,D) ConfigItem<Name> & BOOST_PP_CAT(Arg,Name) ,

AllConfigurationItems::AllConfigurationItems(
#include "ConfigItems.inc"
  Dummy
  )
#undef CONFIGURATION_ITEM
{

#define CONFIGURATION_ITEM(Name,KP,T,DV,D) m_configItems.push_back( & BOOST_PP_CAT(Arg,Name) );
#include "ConfigItems.inc"

}

void
ConfigModule( di::registry & r )
{
  r.add(
    r.type<AllConfigItemsBase>()
    .implementation<AllConfigurationItems>()
    .in_scope<di::scopes::singleton>()
    );
}

int
fromReadable( std::string const & s, Type<int>, boost::mpl::true_ )
{
  return boost::lexical_cast<int>(s);
}

std::string
toReadable( int const & i, Type<int>, boost::mpl::true_ )
{
  return boost::lexical_cast<std::string>(i);
}

boost::filesystem::path
fromReadable( std::string const & s, Type<boost::filesystem::path>, boost::mpl::true_ )
{
  return boost::filesystem::path(s);
}

std::string
toReadable( boost::filesystem::path const & p, Type<boost::filesystem::path>, boost::mpl::true_ )
{
  return p.file_string();
}

