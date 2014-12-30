//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_STOCKQUOTEAPPMODULE_HPP
#define INCLUDED_STOCKQUOTEAPPMODULE_HPP

#include "di/registry.hpp"

struct StockQuoteAppModule
{
  void
  operator()(di::registry & r) const;
};

#endif // STOCKQUOTEAPPMODULE_HPP
