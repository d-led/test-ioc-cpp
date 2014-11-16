// -*- C++ -*-

/**
   CONFIGURATION_ITEM(Name,KeyPath,Type,DefaultValue,Description)
 **/

#define CI CONFIGURATION_ITEM

CI(DocRoot,"config.http.docroot",boost::filesystem::path,".","The default path to use for the docroot")
CI(HttpPort,"config.http.port",int,8080,"Default port for http server")

#undef CI
