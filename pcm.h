#ifndef PCM_H
#define PCM_H

#include <node.h>
#include <node_buffer.h>
#include <alsa/asoundlib.h>

using namespace v8;
using namespace node;

namespace alsa {

class Pcm;

class Pcm : public ObjectWrap {
public:
  static void Init(Handle<Object> exports);

protected:
  Pcm() : ObjectWrap(),
    handle(NULL),
    open(false),
    reading(false) {
  }

  ~Pcm() {
    handle = NULL;
    reading = false;
    open = false;
  }

  struct Baton {
    uv_work_t request;
    Pcm *pcm;

    Baton(Pcm* pcm_) : pcm(pcm_) {
      pcm->Ref();
      request.data = this;
    }
    virtual ~Baton() {
      pcm->Unref();
    }
  };

  struct ReadBaton : Baton {
    char* buffer;
    snd_pcm_uframes_t frames;
    snd_pcm_sframes_t readFrames;

    ReadBaton(Pcm* pcm_, char* buffer_, snd_pcm_uframes_t frames_, snd_pcm_sframes_t readFrames_) :
        Baton(pcm_), buffer(buffer_), frames(frames_), readFrames(readFrames_) {   
    }
  };

  static Handle<Value> New(const Arguments& args);
  static Handle<Value> Open(const Arguments& args);
  static Handle<Value> Close(const Arguments& args);
  static Handle<Value> StartReading(const Arguments& args);
  static Handle<Value> StopReading(const Arguments& args);
  static Handle<Value> ReadingGetter(Local<String> str, const AccessorInfo& accessor);

  static void BeginRead(Baton* baton);
  static void Read(uv_work_t* req);
  static void AfterRead(uv_work_t* req);

  snd_pcm_t *handle;
  bool open;
  bool reading;
};

}

#endif