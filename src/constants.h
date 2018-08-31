
#ifndef NODE_ALSA_CONSTANTS_H_
#define NODE_ALSA_CONSTANTS_H_

#include <alsa/asoundlib.h>
#include <napi.h>

namespace nodealsa {
namespace Constants {

Napi::Object Init(Napi::Env env, Napi::Object exports);

}  // namespace Constants
}  // namespace nodealsa

#endif  // NODE_ALSA_CONSTANTS_H_
