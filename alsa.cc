#include <node.h>
#include <node_buffer.h>
#include "pcm.h"

using namespace v8;
using namespace node;

namespace alsa {

void Init(Handle<Object> exports) {
  Pcm::Init(exports);
}

}

NODE_MODULE(alsa, alsa::Init)