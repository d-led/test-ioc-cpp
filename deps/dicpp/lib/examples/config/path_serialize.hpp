// From http://lists.boost.org/Archives/boost/2008/09/142550.php

#ifndef BOOST_SERIALIZATION_PATH_HPP
#define BOOST_SERIALIZATION_PATH_HPP

#include <boost/config.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/serialization/level.hpp>
#include <boost/serialization/nvp.hpp>

BOOST_CLASS_IMPLEMENTATION(boost::filesystem::path, boost::serialization::primitive_type)
BOOST_CLASS_IMPLEMENTATION(boost::filesystem::wpath, boost::serialization::primitive_type)

namespace boost { namespace serialization {
    template<class Archive, class String, class Traits>
    void serialize(Archive& ar,
                   boost::filesystem::basic_path<String, Traits>& p,
                   const unsigned int version)
    {
      String s;
      if(Archive::is_saving::value)
        s = p.string();
      ar & boost::serialization::make_nvp("string", s);
      if(Archive::is_loading::value)
        p = s;
    }
  }}

#endif
