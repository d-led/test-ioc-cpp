//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_REMOVE_PTR_HPP
#define INCLUDED_REMOVE_PTR_HPP

#include <memory>

namespace di
{
  namespace traits
  {

    template<typename T>
    struct remove_ptr
    {
      typedef T type;
    };

    template<typename T>
    struct remove_ptr<std::shared_ptr<T> >
    {
      typedef T type;
    };

  }
}

#endif // REMOVE_PTR_HPP
