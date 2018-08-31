
#ifndef NODE_ALSA_PCM_H_
#define NODE_ALSA_PCM_H_

#include <alsa/asoundlib.h>
#include <napi.h>
#include <string>

namespace nodealsa {

class Pcm : public Napi::ObjectWrap<Pcm> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Pcm(const Napi::CallbackInfo& info);
  ~Pcm();

 private:
  class Worker : public Napi::AsyncWorker {
   public:
    Worker(const Napi::Object& pcm, const Napi::Promise::Deferred& deferred,
           bool reset)
        : Napi::AsyncWorker(pcm, Napi::Function::New(pcm.Env(), &Noop)),
          _pcm(Pcm::Unwrap(pcm)->_pcm),
          _deferred(deferred),
          _reset(reset),
          _result(0) {}
    ~Worker() {}

    void Execute();
    void OnOK() { _deferred.Resolve(Env().Undefined()); }
    void OnError(const Napi::Error& e);

   private:
    static void Noop(const Napi::CallbackInfo& info) {}

   protected:
    snd_pcm_t* _pcm;
    Napi::Promise::Deferred _deferred;
    bool _reset;
    snd_pcm_uframes_t _result;
    snd_pcm_uframes_t _frames;
    Napi::Reference<Napi::Buffer<uint8_t>> _ref;
    uint8_t* _data;
  };

  class ReadWorker : public Worker {
   public:
    ReadWorker(const Napi::Object& pcm, const Napi::Promise::Deferred& deferred,
               size_t size, bool reset);
    ~ReadWorker() {}
    void OnOK();
  };

  class WriteWorker : public Worker {
   public:
    WriteWorker(const Napi::Object& pcm,
                const Napi::Promise::Deferred& deferred,
                const Napi::Buffer<uint8_t>& buffer, bool reset);
    ~WriteWorker() {}
    void OnOK();
  };

  Napi::Value Read(const Napi::CallbackInfo& info);
  Napi::Value Write(const Napi::CallbackInfo& info);

  snd_pcm_t* _pcm;
};

}  // namespace nodealsa

#endif  // NODE_ALSA_PCM_H_
