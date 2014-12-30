//
// Copyright Somani Software Construction Inc. (2011)
//

#ifndef INCLUDED_EAGER_SINGLETON_HPP_05232011
#define INCLUDED_EAGER_SINGLETON_HPP_05232011

#include "di/scopes/singleton.hpp"

namespace di
{
  namespace scopes
  {

    struct eager_singleton :
      public singleton
    {
    };

  }
}

#endif // INCLUDED_EAGER_SINGLETON_HPP_05232011
