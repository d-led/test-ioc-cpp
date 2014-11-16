//
// Copyright Sohail Somani (2014)
//

#include <boost/test/unit_test.hpp>
#include <string>
#include <ostream>
#include "di/factory.hpp"
#include "di/registry.hpp"

// NOTE: THIS DOESN'T ACTUALLY WORK, IT JUST COMPILES.

struct Payment {
  virtual ~Payment(){}
};

template<>
struct di::factory<Payment> {

  virtual
  std::shared_ptr<Payment>
  create(int date, double amount) = 0;

};

class CreditService{};
class AuthService{};

struct RealPayment : public Payment {
  RealPayment(CreditService & creditService,
              AuthService &   authService,
              int             date,
              double          amount)
  {}
};

BOOST_AUTO_TEST_CASE(Factory) {
  di::registry r;

  // r.add(
  //   r.factory<Payment>()
  //   .implementation<RealPayment>()
  //   .constructor<void(CreditService&,
  //                     AuthService&,
  //                     // Same syntax as type_key
  //                     di::assisted<int>,
  //                     di::assisted<double>)>()
  //   );

}
