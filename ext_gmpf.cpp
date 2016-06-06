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

static Object HHVM_METHOD(GMPf, set, const String &num) {
  auto N = Native::data<GMPf>(this_);
  mpf_set_str(N->val, num.c_str(), 10);
  return Object{this_};
}

static void HHVM_METHOD(GMPf, __construct, const String &num) {
  HHVM_MN(GMPf, set)(this_, num);
}

static String HHVM_METHOD(GMPf, getRaw, VRefParam exponent) {
  auto N = Native::data<GMPf>(this_);
  mp_exp_t exp;
  auto val = mpf_get_str(nullptr, &exp, 10, 0, N->val);
  exponent.assignIfRef((int64_t)exp);
  if (!val) {
    return "0";
  }
  SCOPE_EXIT { free(val); };
  if (!(*val)) return "0";
  return val;
}

using gmpf_binary_op = void (*)(mpf_t, const mpf_t, const mpf_t);

template <gmpf_binary_op OP>
static Object gmpf_method(ObjectData *this_, const Variant& delta) {
  auto N = Native::data<GMPf>(this_);
  mpf_t result;
  mpf_init(result);

  if (delta.isObject() && delta.toObject().instanceof(s_GMPf)) {
    auto other = Native::data<GMPf>(delta.toObject().get());
    OP(result, N->val, other->val);
  } else if (delta.isNumeric()) {
    mpf_t op2;
    mpf_init(op2);
    mpf_set_str(op2, delta.toString().c_str(), 10);
    OP(result, N->val, op2);
    mpf_clear(op2);
  } else {
    SystemLib::throwErrorObject(
      "Invalid argument, expected integer, float, string, or GMPf"
    );
  }

  mpf_swap(N->val, result);
  mpf_clear(result);
  return Object{this_};
}

static struct GMPfExtension : Extension {
  GMPfExtension(): Extension("gmpf", "1.0.0") {}

  void moduleInit() override {
    HHVM_ME(GMPf, __construct);
    HHVM_ME(GMPf, set);
    HHVM_ME(GMPf, getRaw);

    HHVM_NAMED_ME(GMPf, add, gmpf_method<mpf_add>);
    HHVM_NAMED_ME(GMPf, sub, gmpf_method<mpf_sub>);
    HHVM_NAMED_ME(GMPf, mul, gmpf_method<mpf_mul>);
    HHVM_NAMED_ME(GMPf, div, gmpf_method<mpf_div>);

    Native::registerNativeDataInfo<GMPf>(s_GMPf.get());
    loadSystemlib();
  }
} s_gmpf_extension;

HHVM_GET_MODULE(gmpf);
