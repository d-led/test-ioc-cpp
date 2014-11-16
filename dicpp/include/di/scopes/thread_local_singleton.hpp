//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_THREAD_LOCAL_SINGLETON_HPP
#define INCLUDED_THREAD_LOCAL_SINGLETON_HPP

#include "di/scopes/singleton.hpp"
#include "boost/thread/tss.hpp"

namespace di
{
  namespace scopes
  {

    struct thread_local_singleton : public di::scope
    {
    private:
      boost::thread_specific_ptr<singleton> m_instance;

    public:
      virtual
      std::shared_ptr<void>
      construct(di::type_info const & type,
                di::type_provider & unscoped);
    };

  }
}

#endif // THREAD_LOCAL_SINGLETON_HPP
