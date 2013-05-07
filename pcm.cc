#include "pcm.h"

using namespace alsa;

void Pcm::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(String::NewSymbol("Pcm"));

  NODE_SET_PROTOTYPE_METHOD(tpl, "open", Open);
  NODE_SET_PROTOTYPE_METHOD(tpl, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(tpl, "read", Read);
  NODE_SET_PROTOTYPE_METHOD(tpl, "write", Write);

  NODE_SET_GETTER(tpl, "opened", OpenedGetter);
  NODE_SET_GETTER(tpl, "readable", ReadableGetter);
  NODE_SET_GETTER(tpl, "writable", WritableGetter);

  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("Pcm"), constructor);
}

Handle<Value> Pcm::New(const Arguments& args) {
  HandleScope scope;

  if (!args.IsConstructCall())
    return ThrowException(Exception::TypeError(String::New("Use the new operator")));

  if (args.Length() < 4)
    return ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));

  Pcm *pcm = new Pcm();
  pcm->Wrap(args.This());

  args.This()->Set(String::NewSymbol("channels"), args[0], ReadOnly);
  args.This()->Set(String::NewSymbol("rate"), args[1], ReadOnly);
  args.This()->Set(String::NewSymbol("format"), args[2], ReadOnly);
  args.This()->Set(String::NewSymbol("access"), args[3], ReadOnly);
  args.This()->Set(String::NewSymbol("latency"), args[4], ReadOnly);

  return args.This();
}

Handle<Value> Pcm::OpenedGetter(Local<String> str, const AccessorInfo& accessor) {
  HandleScope scope;
  Pcm* pcm = ObjectWrap::Unwrap<Pcm>(accessor.This());
  return Boolean::New(pcm->opened);
}

Handle<Value> Pcm::ReadableGetter(Local<String> str, const AccessorInfo& accessor) {
  HandleScope scope;
  Pcm* pcm = ObjectWrap::Unwrap<Pcm>(accessor.This());
  return Boolean::New(pcm->readable);
}

Handle<Value> Pcm::WritableGetter(Local<String> str, const AccessorInfo& accessor) {
  HandleScope scope;
  Pcm* pcm = ObjectWrap::Unwrap<Pcm>(accessor.This());
  return Boolean::New(pcm->writable);
}

Handle<Value> Pcm::Open(const Arguments& args) {
  HandleScope scope;

  REQUIRE_ARGUMENTS(2);
  OPTIONAL_ARGUMENT_FUNCTION(2, callback);

  Pcm *pcm = ObjectWrap::Unwrap<Pcm>(args.Holder());

  COND_ERR_CALL(pcm->opened, callback, "Already open");
  COND_ERR_CALL(pcm->closing, callback, "Still closing");

  // channels, rate, format, access, latency
  unsigned int channels = args.Holder()->Get(String::NewSymbol("channels"))->Uint32Value();
  unsigned int rate = args.Holder()->Get(String::NewSymbol("rate"))->Uint32Value();
  snd_pcm_format_t format = static_cast<snd_pcm_format_t>(args.Holder()->Get(String::NewSymbol("format"))->Int32Value());
  snd_pcm_access_t access = static_cast<snd_pcm_access_t>(args.Holder()->Get(String::NewSymbol("access"))->Int32Value());
  unsigned int latency = args.Holder()->Get(String::NewSymbol("latency"))->Uint32Value();
  
  // device, stream
  String::Utf8Value device(args[0]->ToString());
  snd_pcm_stream_t stream = static_cast<snd_pcm_stream_t>(args[1]->Int32Value());

  int err;
  err = snd_pcm_open(&(pcm->handle), *device, stream, 0);
  COND_ERR_CALL(err < 0, callback, "Couldn't open");

  err = snd_pcm_set_params(pcm->handle, format, access, channels, rate, 0, latency);
  COND_ERR_CALL(err < 0, callback, "Couldn't set parameters");

#ifdef DEBUG
  static snd_output_t *log;
  snd_output_stdio_attach(&log, stderr, 0);
  snd_pcm_dump(pcm->handle, log);
#endif

  pcm->opened = true;
  pcm->readable = (stream == SND_PCM_STREAM_CAPTURE);
  pcm->writable = (stream == SND_PCM_STREAM_PLAYBACK);
  pcm->interleaved = (access == SND_PCM_ACCESS_RW_INTERLEAVED);

  if (!callback.IsEmpty() && callback->IsFunction())  {
    Local<Value> argv[0] = { };
    TRY_CATCH_CALL(args.Holder(), callback, 0, argv);
  }

  return scope.Close(args.Holder());
}

Handle<Value> Pcm::Close(const Arguments& args) {
  HandleScope scope;

  OPTIONAL_ARGUMENT_FUNCTION(0, callback);

  Pcm *pcm = ObjectWrap::Unwrap<Pcm>(args.Holder());

  COND_ERR_CALL(!pcm->opened, callback, "Not open");
  COND_ERR_CALL(pcm->closing, callback, "Already closing");

  pcm->closing = true;
  Baton *baton = new CloseBaton(pcm, callback);
  BeginClose(baton);

  return scope.Close(args.Holder());
}

void Pcm::BeginClose(Baton* baton) {
  uv_queue_work(uv_default_loop(), &baton->request, DoClose, (uv_after_work_cb)AfterClose);
}

void Pcm::DoClose(uv_work_t* req) {
  sleep(1); // Just wait.
}

void Pcm::AfterClose(uv_work_t* req) {
  HandleScope scope;

  ReadBaton* baton = static_cast<ReadBaton*>(req->data);
  Pcm* pcm = baton->pcm;

  // Wait some more if we're still reading or writing
  if ((pcm->opened && pcm->closing) && (pcm->reading || pcm->writing)) {    
    BeginClose(baton);
    return;
  }

  if (!pcm->opened || !pcm->closing) {
    if (baton->callback.IsEmpty() || !baton->callback->IsFunction()) {
      ThrowException(Exception::TypeError(String::New("Invalid state")));
    } else {
      Local<Value> exception = Exception::Error(String::New("Invalid state"));
      Local<Value> argv[1] = { Local<Value>::New(exception) };
      TRY_CATCH_CALL(pcm->handle_, baton->callback, 1, argv);
    }
    delete baton;
    return;
  }

  if (snd_pcm_drop(pcm->handle) < 0 || 
      snd_pcm_drain(pcm->handle) < 0 || 
      snd_pcm_close(pcm->handle) < 0)
  {
    if (baton->callback.IsEmpty() || !baton->callback->IsFunction()) {
      ThrowException(Exception::TypeError(String::New("Couldn't close")));
    } else {
      Local<Value> exception = Exception::Error(String::New("Couldn't close"));
      Local<Value> argv[1] = { Local<Value>::New(exception) };
      TRY_CATCH_CALL(pcm->handle_, baton->callback, 1, argv);
    }
    delete baton;
    return;
  }

  if (!baton->callback.IsEmpty() && baton->callback->IsFunction()) {
    Local<Value> argv[0] = { };
    TRY_CATCH_CALL(pcm->handle_, baton->callback, 0, argv);
  }

  // Clean up leftovers if we were writing
  if (pcm->leftovers != NULL) {
    free(pcm->leftovers);
    pcm->leftovers = NULL;
    pcm->leftoversLength = 0;
  }

  pcm->opened = false;
  pcm->readable = false;
  pcm->reading = false;
  pcm->writable = false;
  pcm->writing = false;
  pcm->closing = false;
  pcm->handle = NULL;
  delete baton;
}

Handle<Value> Pcm::Read(const Arguments& args) {
  HandleScope scope;

  REQUIRE_ARGUMENT_FUNCTION(0, callback);

  Pcm *pcm = ObjectWrap::Unwrap<Pcm>(args.Holder());

  COND_ERR_CALL(!pcm->opened, callback, "Not open");
  COND_ERR_CALL(!pcm->readable, callback, "Not readable");
  COND_ERR_CALL(pcm->reading, callback, "Already reading");

  snd_pcm_uframes_t bufferSize; // Internal ALSA buffer, not ours (in frames).
  snd_pcm_uframes_t frames;     // Then number of frames in one period at our rate and latency.
  int err;
  err = snd_pcm_get_params(pcm->handle, &bufferSize, &frames);
  COND_ERR_CALL(err < 0, callback, "Couldn't get buffer size");

  pcm->reading = true;
  Baton *baton = new ReadBaton(pcm, callback, frames);
  BeginRead(baton);

  return scope.Close(args.Holder());
}

void Pcm::BeginRead(Baton* baton) {
  // Reset result before reading
  static_cast<ReadBaton*>(baton)->result = 0;
  uv_queue_work(uv_default_loop(), &baton->request, DoRead, (uv_after_work_cb)AfterRead);
}

void Pcm::DoRead(uv_work_t* req) {
  ReadBaton* baton = static_cast<ReadBaton*>(req->data);
  Pcm* pcm = baton->pcm;

  if (pcm->interleaved) {
    baton->result = snd_pcm_readi(pcm->handle, baton->buffer, baton->frames);
  } else {
    // TODO: Figure this out!
    // baton->result = snd_pcm_readn(pcm->handle, &(baton->buffer), baton->frames);
  }
}

void Pcm::AfterRead(uv_work_t* req) {
  HandleScope scope;

  ReadBaton* baton = static_cast<ReadBaton*>(req->data);
  Pcm* pcm = baton->pcm;

  // Silently bail if things were closed out from under us.
  if (!pcm->opened || !pcm->readable || pcm->closing || !pcm->reading) {
    pcm->reading = false;
    delete baton;
    return;
  }

  Local<Value> argv[2] = { Local<Value>::New(Null()), Local<Value>::New(Null()) };

  if (baton->result < 0) {
    // Got a read error - try to recover
    int err = snd_pcm_recover(pcm->handle, static_cast<int>(baton->result), 1);
    // If success just try to read again
    if (err == 0) {
      BeginRead(baton);
      return;
    }
    // Didn't recover - call the callback with the error
    argv[0] = Local<Value>::New(Integer::New(err));
    pcm->reading = false;

  } else if (baton->result < static_cast<snd_pcm_sframes_t>(baton->frames)) {
    // Anything less than the full buffer size is an error
    argv[0] = Local<Value>::New(Integer::New(baton->result));
    pcm->reading = false;

  } else {
    // Success - copy the buffer and pass it to callback and read again
    Buffer *buf = Buffer::New(baton->buffer, snd_pcm_frames_to_bytes(pcm->handle, baton->result));
    argv[1] = Local<Value>::New(buf->handle_);
  }

  // Run the callback
  TRY_CATCH_CALL(pcm->handle_, baton->callback, 2, argv);

  // Go again?
  if (pcm->reading) {
    BeginRead(baton);
  } else {
    delete baton;
  }
}

Handle<Value> Pcm::Write(const Arguments& args) {
  HandleScope scope;

  REQUIRE_ARGUMENTS(2);
  REQUIRE_ARGUMENT_FUNCTION(1, callback);

  Pcm *pcm = ObjectWrap::Unwrap<Pcm>(args.Holder());

  COND_ERR_CALL(!pcm->opened, callback, "Not open");
  COND_ERR_CALL(!pcm->writable, callback, "Not writable");
  COND_ERR_CALL(pcm->writing, callback, "Already writing");

  snd_pcm_uframes_t bufferSize; // Internal ALSA buffer, not ours (in frames).
  snd_pcm_uframes_t frames;     // Then number of frames in one period at our rate and latency.
  int err = snd_pcm_get_params(pcm->handle, &bufferSize, &frames);
  COND_ERR_CALL(err < 0, callback, "Couldn't get buffer size");

  // Bytes in 1 period, which is the size of our buffers
  ssize_t framesBytes = snd_pcm_frames_to_bytes(pcm->handle, frames);

  // Initialize leftovers if we haven't yet.
  if (pcm->leftovers == NULL) {
    pcm->leftovers = (char*)malloc(framesBytes);
    pcm->leftoversLength = 0;
  }

  // Get chunk from args
  char *chunk = Buffer::Data(args[0]->ToObject());
  int chunkLength = Buffer::Length(args[0]->ToObject());

  // Get the total length of all available data
  int totalLength = pcm->leftoversLength + chunkLength;
  snd_pcm_uframes_t chunkFrames = snd_pcm_bytes_to_frames(pcm->handle, static_cast<snd_pcm_uframes_t>(totalLength));

  // We're at least ready to write, starting with leftovers (don't know where chunk will start)
  WriteBaton *baton = new WriteBaton(pcm, callback, frames, NULL, 0);

  // Bail early if we don't have a buffer's worth of data to write
  if (chunkFrames < frames) {
    // Too little data - store entire chunk in leftovers
    memcpy(pcm->leftovers + pcm->leftoversLength, chunk, chunkLength);
    pcm->leftoversLength += chunkLength;

  } else {
    // Copy leftovers
    memcpy(baton->buffer, pcm->leftovers, pcm->leftoversLength);
    // Fill the rest of the buffer from chunk
    int chunkOffset = framesBytes - pcm->leftoversLength;
    memcpy(baton->buffer + pcm->leftoversLength, chunk, chunkOffset);

    // Calculate new leftovers length
    pcm->leftoversLength = totalLength % framesBytes;

    // Copy leftovers from end of chunk
    memcpy(pcm->leftovers, (chunk + chunkLength) - pcm->leftoversLength, pcm->leftoversLength);

    // Bump the chunk up for the next write
    baton->chunk = chunk + chunkOffset;
    baton->chunkFrames = chunkFrames;
  }

  pcm->writing = true;
  BeginWrite(static_cast<Baton*>(baton));

  return scope.Close(args.Holder());
}

void Pcm::BeginWrite(Baton* baton) {
  // Reset result before writing
  static_cast<WriteBaton*>(baton)->result = 0;
  uv_queue_work(uv_default_loop(), &baton->request, DoWrite, (uv_after_work_cb)AfterWrite);
}

void Pcm::DoWrite(uv_work_t* req) {
  WriteBaton* baton = static_cast<WriteBaton*>(req->data);
  Pcm* pcm = baton->pcm;

  if (baton->chunk == NULL) {
    return;
  }

  if (pcm->interleaved) {
    baton->result = snd_pcm_writei(pcm->handle, baton->buffer, baton->frames);
  } else {
    // TODO: Figure this out!
    // baton->result = snd_pcm_writen(pcm->handle, &(baton->buffer), baton->frames);
  }
}

void Pcm::AfterWrite(uv_work_t* req) {
  HandleScope scope;

  WriteBaton* baton = static_cast<WriteBaton*>(req->data);
  Pcm* pcm = baton->pcm;

  Local<Value> argv[1] = { Local<Value>::New(Null()) };

  if (baton->chunk == NULL || !pcm->opened || !pcm->writable || pcm->closing || !pcm->writing) {
    // Just run the callback to keep peeps writing
  } else if (baton->result < 0) {
    // Got a write error - try to recover
    int err = snd_pcm_recover(pcm->handle, static_cast<int>(baton->result), 1);
    // If success just try to write again
    if (err == 0) {
      BeginWrite(baton);
      return;
    }
    // Didn't recover - call the callback with the error
    argv[0] = Local<Value>::New(Integer::New(err));

  } else if (baton->result < static_cast<snd_pcm_sframes_t>(baton->frames)) {
    // Anything less than the full buffer size is an error
    argv[0] = Local<Value>::New(Integer::New(baton->result));

  } else if (baton->result + baton->total < baton->chunkFrames) {
    // We still have data
    ssize_t framesBytes = snd_pcm_frames_to_bytes(pcm->handle, baton->frames);
    baton->total += baton->result;
    memcpy(baton->buffer, baton->chunk, framesBytes);

    // Bump chunk (if it's not the last one)
    if (baton->total + baton->frames < baton->chunkFrames) {
      baton->chunk += framesBytes;
    }

    BeginWrite(baton);
    return;
  }

  // Run the callback (which may set writing again in this thread)
  pcm->writing = false;
  TRY_CATCH_CALL(pcm->handle_, baton->callback, 1, argv);
  delete baton;
}