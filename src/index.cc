
#include <napi.h>
#include "constants.h"
#include "pcm.h"

namespace nodealsa {

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports = Constants::Init(env, exports);
  return Pcm::Init(env, exports);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace nodealsa
