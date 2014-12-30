//
// Copyright Sohail Somani (2014)
//

#ifndef INCLUDED_IMPLEMENTATION_HPP_06042014
#define INCLUDED_IMPLEMENTATION_HPP_06042014

namespace di
{
  namespace traits
  {

    template<typename BaseType>
    struct implementation
    {
      typedef BaseType type;
    };

  }
}

#endif // INCLUDED_IMPLEMENTATION_HPP_06042014
