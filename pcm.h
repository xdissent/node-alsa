#ifndef PCM_H
#define PCM_H

#include <node.h>
#include <node_buffer.h>
#include <alsa/asoundlib.h>
#include "macros.h"

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
    opened(false),
    readable(false),
    reading(false),
    writable(false),
    writing(false),
    interleaved(false),
    closing(false),
    leftovers(NULL),
    leftoversLength(0) {
  }

  ~Pcm() {
    handle = NULL;
    opened = false;
    readable = false;
    reading = false;
    writable = false;
    writing = false;
    interleaved = false;
    closing = false;
    leftovers = NULL;
    leftoversLength = 0;
  }

  struct Baton {
    uv_work_t request;
    Pcm *pcm;
    Persistent<Function> callback;

    Baton(Pcm* pcm_, Handle<Function> cb_) : 
        pcm(pcm_) {
      pcm->Ref();
      request.data = this;
      callback = Persistent<Function>::New(cb_);
    }
    virtual ~Baton() {
      pcm->Unref();
      callback.Dispose();
    }
  };

  struct CloseBaton : Baton {
    CloseBaton(Pcm* pcm_, Handle<Function> cb_) : 
        Baton(pcm_, cb_) {
    }
  };

  struct IOBaton : Baton {
    snd_pcm_uframes_t frames;
    snd_pcm_sframes_t result;
    char* buffer;

    IOBaton(Pcm* pcm_, Handle<Function> cb_, snd_pcm_uframes_t frames_) : 
        Baton(pcm_, cb_), frames(frames_), result(0), buffer(NULL) {
      buffer = (char*)malloc(snd_pcm_frames_to_bytes(pcm->handle, frames));
    }
    virtual ~IOBaton() {
      free(buffer);
    }
  };

  struct ReadBaton : IOBaton {
    ReadBaton(Pcm* pcm_, Handle<Function> cb_, snd_pcm_uframes_t frames_) : 
        IOBaton(pcm_, cb_, frames_) {
    }
  };

  struct WriteBaton : IOBaton {
    char* chunk;
    snd_pcm_uframes_t chunkFrames;
    snd_pcm_uframes_t total;

    WriteBaton(Pcm* pcm_, Handle<Function> cb_, snd_pcm_uframes_t frames_, char* chunk_, snd_pcm_uframes_t chunkFrames_) : 
        IOBaton(pcm_, cb_, frames_), chunk(chunk_), chunkFrames(chunkFrames_), total(0) {
    }
  };

  static Handle<Value> New(const Arguments& args);
  static Handle<Value> Open(const Arguments& args);
  static Handle<Value> Close(const Arguments& args);
  static Handle<Value> Read(const Arguments& args);
  static Handle<Value> Write(const Arguments& args);

  static Handle<Value> OpenedGetter(Local<String> str, const AccessorInfo& accessor);
  static Handle<Value> ReadableGetter(Local<String> str, const AccessorInfo& accessor);
  static Handle<Value> WritableGetter(Local<String> str, const AccessorInfo& accessor);

  static void BeginRead(Baton* baton);
  static void DoRead(uv_work_t* req);
  static void AfterRead(uv_work_t* req);

  static void BeginClose(Baton* baton);
  static void DoClose(uv_work_t* req);
  static void AfterClose(uv_work_t* req);

  static void BeginWrite(Baton* baton);
  static void DoWrite(uv_work_t* req);
  static void AfterWrite(uv_work_t* req);

  snd_pcm_t *handle;
  bool opened;
  bool readable;
  bool reading;
  bool writable;
  bool writing;
  bool interleaved;
  bool closing;
  char *leftovers;
  int leftoversLength;
};

}

#endif