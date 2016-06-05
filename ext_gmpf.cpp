#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/vm/native-data.h"

#include <gmp.h>

using namespace HPHP;

const StaticString s_GMPf("GMPf");
struct GMPf {
  GMPf() {
    mpf_init(val);
  }
  GMPf& operator=(const GMPf &src) {
    mpf_init(val);
    mpf_set(val, src.val);
    return *this;
  }
  ~GMPf() {
    mpf_clear(val);
  }

  mpf_t val;
};

static void HHVM_METHOD(GMPf, __construct, const Variant &num) {
  auto N = Native::data<GMPf>(this_);
  if (num.isInteger()) {
    mpf_set_si(N->val, num.toInt64());
  } else if (num.isDouble()) {
    mpf_set_d(N->val, num.toDouble());
  } else if (num.isString()) {
    mpf_set_str(N->val, num.toString().c_str(), 10);
  } else if (!num.isNull()) {
    SystemLib::throwErrorObject(String("Invalid arg to GMPf constructor"));
  }
}

static String HHVM_METHOD(GMPf, __toString) {
  auto N = Native::data<GMPf>(this_);
  mp_exp_t exp;
  auto val = mpf_get_str(nullptr, &exp, 10, 0, N->val);
  if (val) {
    SCOPE_EXIT { free(val); };
    if (!(*val)) return "0";
    // Ugly reformat to 0.1234E2
    String ret("0.");
    ret += val;
    if (!exp) return ret;
    ret += "E";
    if (exp > 0) ret += "+";
    return ret + exp;
  } else {
    return empty_string();
  }
}

static struct GMPfExtension : Extension {
  GMPfExtension(): Extension("gmpf", "1.0.0") {}

  void moduleInit() override {
    HHVM_ME(GMPf, __construct);
    HHVM_ME(GMPf, __toString);

    Native::registerNativeDataInfo<GMPf>(s_GMPf.get());
    loadSystemlib();
  }
} s_gmpf_extension;

HHVM_GET_MODULE(gmpf);
