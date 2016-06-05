#include "hphp/runtime/ext/extension.h"

using namespace HPHP;

static struct GMPfExtension : Extension {
  GMPfExtension(): Extension("gmpf", "1.0.0") {}

  void moduleInit() override {
    loadSystemlib();
  }
} s_gmpf_extension;

HHVM_GET_MODULE(gmpf);
