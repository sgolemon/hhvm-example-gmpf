#include "hphp/runtime/ext/extension.h"

#include <gmp.h>

using namespace HPHP;

static double HHVM_FUNCTION(gmpf_divide, double numer, double denom) {
  mpf_t n, d, r;
  mpf_inits(n, d, r, nullptr);

  mpf_set_d(n, numer);
  mpf_set_d(d, denom);

  mpf_div(r, n, d);

  auto ret = mpf_get_d(r);
  mpf_clears(n, d, r, nullptr);

  return ret;
}


static struct GMPfExtension : Extension {
  GMPfExtension(): Extension("gmpf", "1.0.0") {}

  void moduleInit() override {
    HHVM_FE(gmpf_divide);
    loadSystemlib();
  }
} s_gmpf_extension;

HHVM_GET_MODULE(gmpf);
