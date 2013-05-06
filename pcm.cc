#include "pcm.h"

using namespace alsa;

void Pcm::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(String::NewSymbol("Pcm"));

  NODE_SET_PROTOTYPE_METHOD(tpl, "open", Open);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "startReading", StartReading);
  NODE_SET_PROTOTYPE_METHOD(tpl, "stopReading", StopReading);

  tpl->InstanceTemplate()->SetAccessor(String::NewSymbol("reading"), ReadingGetter);

  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("Pcm"), constructor);
}

Handle<Value> Pcm::New(const Arguments& args) {
  HandleScope scope;

  if (!args.IsConstructCall()) {
    ThrowException(Exception::TypeError(String::New("Use the new operator")));
    return scope.Close(Undefined());
  }

  if (args.Length() < 4) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  Pcm *pcm = new Pcm();
  pcm->Wrap(args.This());

  args.This()->Set(String::NewSymbol("channels"), args[0], ReadOnly);
  args.This()->Set(String::NewSymbol("rate"), args[1], ReadOnly);
  args.This()->Set(String::NewSymbol("format"), args[2], ReadOnly);
  args.This()->Set(String::NewSymbol("access"), args[3], ReadOnly);

  return args.This();
}

Handle<Value> Pcm::Open(const Arguments& args) {
  HandleScope scope;

  Pcm *pcm = ObjectWrap::Unwrap<Pcm>(args.Holder());

  if (pcm->open) {
    ThrowException(Exception::TypeError(String::New("Already open")));
    return scope.Close(Undefined());
  }

  if (args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  // channels, rate, format, access
  unsigned int channels = args.Holder()->Get(String::NewSymbol("channels"))->Uint32Value();
  unsigned int rate = args.Holder()->Get(String::NewSymbol("rate"))->Uint32Value();
  snd_pcm_format_t format = static_cast<snd_pcm_format_t>(args.Holder()->Get(String::NewSymbol("format"))->Int32Value());
  snd_pcm_access_t access = static_cast<snd_pcm_access_t>(args.Holder()->Get(String::NewSymbol("access"))->Int32Value());

  // device, stream
  String::Utf8Value device(args[0]->ToString());
  snd_pcm_stream_t stream = static_cast<snd_pcm_stream_t>(args[1]->Int32Value());

  int err;
  err = snd_pcm_open(&(pcm->handle), *device, stream, 0);
  if (err < 0) {
    ThrowException(Exception::TypeError(String::New("Couldn't open")));
    return scope.Close(Undefined());
  }

  snd_pcm_hw_params_t *params;
  snd_pcm_hw_params_alloca(&params);
  err = snd_pcm_hw_params_any(pcm->handle, params);
  if (err < 0) {
    ThrowException(Exception::TypeError(String::New("No device")));
    return scope.Close(Undefined());
  }
  err = snd_pcm_hw_params_set_access(pcm->handle, params, access);
  if (err < 0) {
    ThrowException(Exception::TypeError(String::New("Invalid access")));
    return scope.Close(Undefined());
  }
  err = snd_pcm_hw_params_set_format(pcm->handle, params, format);
  if (err < 0) {
    ThrowException(Exception::TypeError(String::New("Invalid format")));
    return scope.Close(Undefined());
  }
  err = snd_pcm_hw_params_set_channels(pcm->handle, params, channels);
  if (err < 0) {
    ThrowException(Exception::TypeError(String::New("Invalid channels")));
    return scope.Close(Undefined());
  }
  err = snd_pcm_hw_params_set_rate_near(pcm->handle, params, &rate, NULL);
  if (err < 0) {
    ThrowException(Exception::TypeError(String::New("Invalid rate")));
    return scope.Close(Undefined());
  }
  err = snd_pcm_hw_params(pcm->handle, params);
  if (err < 0) {
    ThrowException(Exception::TypeError(String::New("Unable to set params")));
    return scope.Close(Undefined());
  }

  pcm->open = true;

  return scope.Close(args.Holder());
}

Handle<Value> Pcm::Close(const Arguments& args) {
  HandleScope scope;

  Pcm *pcm = ObjectWrap::Unwrap<Pcm>(args.Holder());

  if (!pcm->open) {
    ThrowException(Exception::TypeError(String::New("Not open")));
  }

  int err;
  err = snd_pcm_drop(pcm->handle);
  if (err < 0) {
    ThrowException(Exception::TypeError(String::New("Couldn't drop")));
  }

  err = snd_pcm_drain(pcm->handle);
  if (err < 0) {
    ThrowException(Exception::TypeError(String::New("Couldn't drain")));
  }

  err = snd_pcm_close(pcm->handle);
  if (err < 0) {
    ThrowException(Exception::TypeError(String::New("Couldn't close")));
  }

  pcm->open = false;
  pcm->handle = NULL;

  return scope.Close(args.Holder());
}

Handle<Value> Pcm::StartReading(const Arguments& args) {
  HandleScope scope;

  Pcm *pcm = ObjectWrap::Unwrap<Pcm>(args.Holder());

  if (!pcm->open) {
    ThrowException(Exception::TypeError(String::New("Not open")));
    return scope.Close(Undefined());
  }

  if (pcm->reading) {
    ThrowException(Exception::TypeError(String::New("Already reading")));
    return scope.Close(Undefined());
  }

  // Build a buffer
  snd_pcm_uframes_t frames = 5513;
  char* buffer = (char*)malloc(snd_pcm_frames_to_bytes(pcm->handle, frames));
  if (buffer == NULL) {
    ThrowException(Exception::TypeError(String::New("Not enough memory")));
    return scope.Close(Undefined());
  }

  Baton *baton = new ReadBaton(pcm, buffer, frames, 0);
  pcm->reading = true;
  BeginRead(baton);

  return scope.Close(args.Holder());
}

Handle<Value> Pcm::StopReading(const Arguments& args) {
  HandleScope scope;

  Pcm *pcm = ObjectWrap::Unwrap<Pcm>(args.Holder());

  if (!pcm->reading) {
    ThrowException(Exception::TypeError(String::New("Not reading")));
    return scope.Close(Undefined());
  }

  pcm->reading = false;

  return scope.Close(args.Holder());
}

Handle<Value> Pcm::ReadingGetter(Local<String> str, const AccessorInfo& accessor) {
  HandleScope scope;
  Pcm* pcm = ObjectWrap::Unwrap<Pcm>(accessor.This());
  return Boolean::New(pcm->reading);
}

void Pcm::BeginRead(Baton* baton) {
  uv_queue_work(uv_default_loop(), &baton->request, Read, (uv_after_work_cb)AfterRead);
}

void Pcm::Read(uv_work_t* req) {
  ReadBaton* baton = static_cast<ReadBaton*>(req->data);
  Pcm* pcm = baton->pcm;

  if (pcm->open && pcm->reading) {
    baton->readFrames = snd_pcm_readi(pcm->handle, baton->buffer, baton->frames);
  }
}

void Pcm::AfterRead(uv_work_t* req) {
  HandleScope scope;
  ReadBaton* baton = static_cast<ReadBaton*>(req->data);
  Pcm* pcm = baton->pcm;

  Local<Value> onDataVal = pcm->handle_->Get(String::NewSymbol("onData"));

  if (pcm->open && pcm->reading && baton->readFrames > 0 && onDataVal->IsFunction()) {
    TryCatch try_catch;

    Local<Function> onData = Local<Function>::Cast(onDataVal);
    Buffer *buf = Buffer::New(baton->buffer, snd_pcm_frames_to_bytes(pcm->handle, baton->readFrames));
    Local<Value> argv[1] = { Local<Value>::New(buf->handle_) };
    
    onData->Call(pcm->handle_, 1, argv);

    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }

    baton->readFrames = 0;
    BeginRead(baton);
    return;
  }

  Local<Value> onEndVal = pcm->handle_->Get(String::NewSymbol("onEnd"));
  if (onEndVal->IsFunction()) {
    TryCatch try_catch;

    Local<Function> onEnd = Local<Function>::Cast(onEndVal);
    Local<Value> argv[0] = { };
    onEnd->Call(pcm->handle_, 0, argv);

    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }
  }

  free(baton->buffer);
  delete baton;
}