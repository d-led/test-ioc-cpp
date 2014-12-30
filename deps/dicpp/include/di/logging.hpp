//
// Copyright Somani Software Construction Inc. (2010)
//

#ifndef INCLUDED_LOGGING_HPP_11062010
#define INCLUDED_LOGGING_HPP_11062010

#include <iostream>

// Define this macro if you want to use logging
#if !defined(DI_DISABLE_LOGGING)
#  define DI_LOG_DEBUG(x) std::cout << "[DICPP]: " << x << std::endl
#else
#  define DI_LOG_DEBUG(x) (void)0
#endif

#endif // INCLUDED_LOGGING_HPP_11062010
