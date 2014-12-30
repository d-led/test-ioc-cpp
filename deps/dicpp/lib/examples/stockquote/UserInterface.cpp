//
// Copyright Somani Software Construction Inc. 2010
//

#include "UserInterface.hpp"

void
UserInterface::execute(std::function<void()> const & fn)
{
  fn();
}
