//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_CONSTRUCTOR_HPP
#define INCLUDED_CONSTRUCTOR_HPP

#include "di/type_key.hpp"

#define DI_CONSTRUCTOR(Type,Signature)          \
  DI_CONSTRUCTOR_TYPES(Signature);              \
  Type Signature

// Use this version in class templates
#define DI_CONSTRUCTOR_T(Type,Signature)        \
  DI_CONSTRUCTOR_TYPES_T(Signature);            \
  Type Signature

#define DI_CONSTRUCTOR_TYPES(Signature)         \
  typedef di::type_key< void Signature >::type constructor

#define DI_CONSTRUCTOR_TYPES_T(Signature)       \
  typedef typename di::type_key< void Signature >::type constructor

#endif // CONSTRUCTOR_HPP
