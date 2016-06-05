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

static String HHVM_METHOD(GMPf, get) {
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

static Object HHVM_METHOD(GMPf, add, const Variant& delta) {
  auto N = Native::data<GMPf>(this_);
  mpf_t result;
  mpf_init(result);

  if (delta.isInteger()) {
    mpf_add_ui(result, N->val, delta.toInt64());
  } else if (delta.isObject() && delta.toObject().instanceof(s_GMPf)) {
    auto other = Native::data<GMPf>(delta.toObject().get());
    mpf_add(result, N->val, other->val);
  } else {
    SystemLib::throwErrorObject("Invalid argument");
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
    HHVM_ME(GMPf, get);
    HHVM_ME(GMPf, add);

    Native::registerNativeDataInfo<GMPf>(s_GMPf.get());
    loadSystemlib();
  }
} s_gmpf_extension;

HHVM_GET_MODULE(gmpf);
