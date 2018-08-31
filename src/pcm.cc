
#include "pcm.h"
#include <alsa/asoundlib.h>
#include <napi.h>
#include <chrono>
#include <stdexcept>
#include <system_error>
#include <thread>

namespace nodealsa {

void Pcm::Worker::Execute() {
  int ret;
  snd_pcm_uframes_t buffer;
  snd_pcm_uframes_t period;
  snd_pcm_sframes_t available;
  snd_pcm_sframes_t frames;
  ssize_t bytes;
  bool capture = snd_pcm_stream(_pcm) == SND_PCM_STREAM_CAPTURE;
  if (_reset) {
    ret = snd_pcm_prepare(_pcm);
    if (ret < 0) throw std::system_error(-ret, std::generic_category());
    if (capture) {
      ret = snd_pcm_start(_pcm);
      if (ret < 0) throw std::system_error(-ret, std::generic_category());
    }
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
  ret = snd_pcm_get_params(_pcm, &buffer, &period);
  if (ret < 0) throw std::system_error(-ret, std::generic_category());
  do {
    available = snd_pcm_avail(_pcm);
    if (available < 0)
      throw std::system_error(-available, std::generic_category());
    if (available == 0) {
      std::this_thread::sleep_for(std::chrono::microseconds(10));
      continue;
    }
    if ((snd_pcm_uframes_t)available > _frames - _result)
      available = _frames - _result;
    bytes = snd_pcm_frames_to_bytes(_pcm, _result);
    if (bytes < 0) throw std::system_error(-bytes, std::generic_category());
    frames = capture ? snd_pcm_readi(_pcm, _data + bytes, available)
                     : snd_pcm_writei(_pcm, _data + bytes, available);
    if (frames < 0) throw std::system_error(-frames, std::generic_category());
    _result += frames;
    if (_result > _frames) throw std::runtime_error("Too many frames");
    if (_result == _frames) break;
    if (capture && _result >= period) break;
  } while (true);
}

void Pcm::Worker::OnError(const Napi::Error& e) { _deferred.Reject(e.Value()); }

Pcm::ReadWorker::ReadWorker(const Napi::Object& pcm,
                            const Napi::Promise::Deferred& deferred,
                            size_t size, bool reset)
    : Worker(pcm, deferred, reset) {
  _frames = snd_pcm_bytes_to_frames(_pcm, size);
  ssize_t bytes = snd_pcm_frames_to_bytes(_pcm, _frames);
  if (bytes <= 0) throw Napi::TypeError::New(Env(), "Invalid size");
  if ((size_t)bytes > size) _frames--;
  bytes = snd_pcm_frames_to_bytes(_pcm, _frames);
  if (bytes <= 0) throw Napi::TypeError::New(Env(), "Invalid size");
  Napi::Buffer<uint8_t> buffer = Napi::Buffer<uint8_t>::New(Env(), bytes);
  _ref = Napi::Persistent(buffer);
  _data = buffer.Data();
}

void Pcm::ReadWorker::OnOK() {
  try {
    Napi::Buffer<uint8_t> buffer = _ref.Value().As<Napi::Buffer<uint8_t>>();
    Napi::Function slice = buffer.Get("slice").As<Napi::Function>();
    ssize_t size = snd_pcm_frames_to_bytes(_pcm, _result);
    Napi::Value sliced = slice.Call(buffer, {
                                                Napi::Number::New(Env(), 0),
                                                Napi::Number::New(Env(), size),
                                            });
    _deferred.Resolve(sliced);
  } catch (const Napi::Error& e) {
    _deferred.Reject(e.Value());
  }
}

Pcm::WriteWorker::WriteWorker(const Napi::Object& pcm,
                              const Napi::Promise::Deferred& deferred,
                              const Napi::Buffer<uint8_t>& buffer, bool reset)
    : Worker(pcm, deferred, reset) {
  _frames = snd_pcm_bytes_to_frames(_pcm, buffer.Length());
  ssize_t bytes = snd_pcm_frames_to_bytes(_pcm, _frames);
  if (bytes <= 0) throw Napi::TypeError::New(Env(), "Invalid size");
  if ((size_t)bytes > buffer.Length()) _frames--;
  _ref = Napi::Persistent(buffer);
  _data = buffer.Data();
}

void Pcm::WriteWorker::OnOK() {
  try {
    ssize_t size = snd_pcm_frames_to_bytes(_pcm, _result);
    _deferred.Resolve(Napi::Number::New(Env(), size));
  } catch (const Napi::Error& e) {
    _deferred.Reject(e.Value());
  }
}

Napi::Object Pcm::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);
  Napi::Function cls = DefineClass(env, "Pcm",
                                   {
                                       InstanceMethod("read", &Pcm::Read),
                                       InstanceMethod("write", &Pcm::Write),
                                   });
  exports.Set("Pcm", cls);
  return exports;
}

Pcm::Pcm(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Pcm>(info) {
  if (info.Length() != 1 || !info[0].IsObject())
    throw Napi::TypeError::New(info.Env(), "Invalid options");

  Napi::Object options = info[0].As<Napi::Object>();
  std::string device = options.Get("device").As<Napi::String>();
  snd_pcm_stream_t stream =
      (snd_pcm_stream_t)((uint32_t)options.Get("stream").As<Napi::Number>());
  snd_pcm_format_t format =
      (snd_pcm_format_t)((uint32_t)options.Get("format").As<Napi::Number>());
  snd_pcm_access_t access =
      (snd_pcm_access_t)((uint32_t)options.Get("access").As<Napi::Number>());
  // TODO: Support NI?
  if (access != SND_PCM_ACCESS_RW_INTERLEAVED)
    throw Napi::TypeError::New(info.Env(), "Unsupported access");
  unsigned int channels = options.Get("channels").As<Napi::Number>();
  unsigned int rate = options.Get("rate").As<Napi::Number>();
  unsigned int resample = options.Get("resample").As<Napi::Boolean>();
  unsigned int latency = options.Get("latency").As<Napi::Number>();

  int ret;
  ret = snd_pcm_open(&_pcm, (char*)device.c_str(), stream, 0);
  if (ret < 0) {
    throw Napi::TypeError::New(info.Env(), "Device open failed");
  }

  ret = snd_pcm_set_params(_pcm, format, access, channels, rate, resample,
                           latency);
  if (ret < 0) {
    snd_pcm_close(_pcm);
    throw Napi::TypeError::New(info.Env(), "Device params failed");
  }
}

Pcm::~Pcm() {
  snd_pcm_drop(_pcm);
  snd_pcm_drain(_pcm);
  snd_pcm_close(_pcm);
}

Napi::Value Pcm::Read(const Napi::CallbackInfo& info) {
  Pcm::ReadWorker* worker = nullptr;
  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(info.Env());
  try {
    int32_t size = info[0].As<Napi::Number>();
    if (size <= 0) throw Napi::TypeError::New(info.Env(), "Invalid size");
    worker = new ReadWorker(info.This().As<Napi::Object>(), deferred,
                            (size_t)size, info[1].As<Napi::Boolean>());
    worker->Queue();
  } catch (const Napi::Error& e) {
    if (worker) delete worker;
    deferred.Reject(e.Value());
  }
  return deferred.Promise();
}

Napi::Value Pcm::Write(const Napi::CallbackInfo& info) {
  Pcm::WriteWorker* worker = nullptr;
  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(info.Env());
  try {
    worker = new WriteWorker(info.This().As<Napi::Object>(), deferred,
                             info[0].As<Napi::Buffer<uint8_t>>(),
                             info[1].As<Napi::Boolean>());
    worker->Queue();
  } catch (const Napi::Error& e) {
    if (worker) delete worker;
    deferred.Reject(e.Value());
  }
  return deferred.Promise();
}

}  // namespace nodealsa
