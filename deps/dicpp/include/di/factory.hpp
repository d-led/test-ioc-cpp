//
// Copyright Sohail Somani (2014)
//

#ifndef INCLUDED_FACTORY_HPP_04212014
#define INCLUDED_FACTORY_HPP_04212014

#include "di/type_key.hpp"

namespace di
{
  /**
     Note: this doesn't actually work yet. Documenting for possible
     future work.

     A factory is a kind of parameterized provider: some objects
     require parameters for construction that cannot really be
     provided via injection.

     Usage (the example is from Guice docs):

  struct Payment {
    virtual ~Payment(){}
  };

  struct RealPayment : public Payment {
    RealPayment( CreditService & creditService, // injected
                 AuthService &   authService,   // injected
                 Date            date,
                 Money           amount );
  };

  template<>
  struct di::factory<Payment> {

    virtual
    std::shared_ptr<Payment>
    create(int date, double amount) const = 0;

  };

  void function () {
    di::registry r;
    // ...
    r.add(
      r.factory<Payment>()
      .implementation<RealPayment>()
      .constructor<void(CreditService&,
                        AuthService&,
                        assisted<int> date,
                        assisted<double> amount)>()
      );

    // ...
    auto factory = r.construct_ptr<di::factory<Payment>>();
    factory->create(now(),orderTotal);
  }
  **/
  template<typename T, typename Annotation = void>
  struct assisted : public type_key<T,Annotation>
  {};

  template<typename T>
  struct factory;
}

#endif // INCLUDED_FACTORY_HPP_04212014
