This example shows how you can use dependency injection to inject
application configuration.

The configuration is "stored" in the InMemoryStore class but you can
imagine it being stored in some kind of disk-backed key-value store.

The application is a simple HTTP server with two configuration values:

* docroot
* port

An example session:

What is thy bidding, my master?: config

*****
config.http.docroot The default path to use for the docroot. Current value: .
*****

*****
config.http.port Default port for http server. Current value: 8080
*****

What is thy bidding, my master?: config set config.http.port 3030
Starting server on port 3030 serving from .

*****
Config item config.http.port has value 3030
*****

The HTTP server itself accepts these configuration items and then
listens for changes to the configuration (so it can restart itself):

// File: AsioHttpServer.hpp

  DI_CONSTRUCTOR(AsioHttpServer,(
                   ConfigItem<DocRoot> &,
                   ConfigItem<HttpPort> &
                   ));

// File: AsioHttpServer.cpp

AsioHttpServer::AsioHttpServer( ConfigItem<DocRoot> & docRoot,
                                ConfigItem<HttpPort> & httpPort )
  : m (new Impl)
{
  m->docRoot  = docRoot.getValue();
  m->httpPort = httpPort.getValue();

  docRoot.signalValueChanged().connect(boost::bind(&Impl::handleDocrootChange,m,_1,boost::ref(*this)));
  httpPort.signalValueChanged().connect(boost::bind(&Impl::handlePortChange,m,_1,boost::ref(*this)));
}

To facilitate iteration over all configuration items, each config item
is defined in a header file:

// ConfigItems.inc

#define CI CONFIGURATION_ITEM

CI(DocRoot,"config.http.docroot",boost::filesystem::path,".","The default path to use for the docroot")
CI(HttpPort,"config.http.port",int,8080,"Default port for http server")

Then, to make use of these config items, you can define the symbol
CONFIGURATION_ITEM and #include "ConfigItems.inc":

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

The above class (AllConfigurationItems) actually exists to facilitate
generic iteration. It has a list of all configuration items that can
be set and read generically.

If you use dicpp in your apps, you can probably copy the Config* and
*Store* files, modifying ConfigItems.inc as you see fit.
