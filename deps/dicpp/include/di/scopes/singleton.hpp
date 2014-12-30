//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_SINGLETON_HPP
#define INCLUDED_SINGLETON_HPP

#include <map>
#include "di/scope.hpp"
#include "boost/thread/recursive_mutex.hpp"

namespace di
{
  namespace scopes
  {
    /**
       Be very careful when using this. The rule of thumb is: do not
       use providers in the constructor of an object or in any
       function that is called by the constructor.

       If you can't avoid that then most definitely do not ever under
       any circumstances run a thread in a constructor and use a
       provider in the new thread.
     **/
    struct singleton : public di::scope
    {
    private:
      typedef std::map<di::type_info,std::shared_ptr<void> > instance_map_t;
      instance_map_t         m_instances;
      boost::recursive_mutex m_mutex;

    public:
      virtual
      std::shared_ptr<void>
      construct(di::type_info const & type,
                di::type_provider & unscoped);

    };
  }

}


#endif // SINGLETON_HPP
