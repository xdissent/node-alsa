
#include "constants.h"
#include <alsa/asoundlib.h>
#include <napi.h>

namespace nodealsa {
namespace Constants {

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Napi::Object constants = Napi::Object::New(env);

  constants.DefineProperties({
      Napi::PropertyDescriptor::Value(
          "CLASS_GENERIC", Napi::Number::New(env, SND_PCM_CLASS_GENERIC),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "CLASS_MULTI", Napi::Number::New(env, SND_PCM_CLASS_MULTI),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "CLASS_MODEM", Napi::Number::New(env, SND_PCM_CLASS_MODEM),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "CLASS_DIGITIZER", Napi::Number::New(env, SND_PCM_CLASS_DIGITIZER),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "CLASS_LAST", Napi::Number::New(env, SND_PCM_CLASS_LAST),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "SUBCLASS_GENERIC_MIX",
          Napi::Number::New(env, SND_PCM_SUBCLASS_GENERIC_MIX),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "SUBCLASS_MULTI_MIX",
          Napi::Number::New(env, SND_PCM_SUBCLASS_MULTI_MIX), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "SUBCLASS_LAST", Napi::Number::New(env, SND_PCM_SUBCLASS_LAST),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STREAM_PLAYBACK", Napi::Number::New(env, SND_PCM_STREAM_PLAYBACK),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STREAM_CAPTURE", Napi::Number::New(env, SND_PCM_STREAM_CAPTURE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STREAM_LAST", Napi::Number::New(env, SND_PCM_STREAM_LAST),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "ACCESS_MMAP_INTERLEAVED",
          Napi::Number::New(env, SND_PCM_ACCESS_MMAP_INTERLEAVED),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "ACCESS_MMAP_NONINTERLEAVED",
          Napi::Number::New(env, SND_PCM_ACCESS_MMAP_NONINTERLEAVED),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "ACCESS_MMAP_COMPLEX",
          Napi::Number::New(env, SND_PCM_ACCESS_MMAP_COMPLEX), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "ACCESS_RW_INTERLEAVED",
          Napi::Number::New(env, SND_PCM_ACCESS_RW_INTERLEAVED),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "ACCESS_RW_NONINTERLEAVED",
          Napi::Number::New(env, SND_PCM_ACCESS_RW_NONINTERLEAVED),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "ACCESS_LAST", Napi::Number::New(env, SND_PCM_ACCESS_LAST),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_UNKNOWN", Napi::Number::New(env, SND_PCM_FORMAT_UNKNOWN),
          napi_enumerable),
      Napi::PropertyDescriptor::Value("FORMAT_S8",
                                      Napi::Number::New(env, SND_PCM_FORMAT_S8),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value("FORMAT_U8",
                                      Napi::Number::New(env, SND_PCM_FORMAT_U8),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S16_LE", Napi::Number::New(env, SND_PCM_FORMAT_S16_LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S16_BE", Napi::Number::New(env, SND_PCM_FORMAT_S16_BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U16_LE", Napi::Number::New(env, SND_PCM_FORMAT_U16_LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U16_BE", Napi::Number::New(env, SND_PCM_FORMAT_U16_BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S24_LE", Napi::Number::New(env, SND_PCM_FORMAT_S24_LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S24_BE", Napi::Number::New(env, SND_PCM_FORMAT_S24_BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U24_LE", Napi::Number::New(env, SND_PCM_FORMAT_U24_LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U24_BE", Napi::Number::New(env, SND_PCM_FORMAT_U24_BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S32_LE", Napi::Number::New(env, SND_PCM_FORMAT_S32_LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S32_BE", Napi::Number::New(env, SND_PCM_FORMAT_S32_BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U32_LE", Napi::Number::New(env, SND_PCM_FORMAT_U32_LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U32_BE", Napi::Number::New(env, SND_PCM_FORMAT_U32_BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_FLOAT_LE", Napi::Number::New(env, SND_PCM_FORMAT_FLOAT_LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_FLOAT_BE", Napi::Number::New(env, SND_PCM_FORMAT_FLOAT_BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_FLOAT64_LE",
          Napi::Number::New(env, SND_PCM_FORMAT_FLOAT64_LE), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_FLOAT64_BE",
          Napi::Number::New(env, SND_PCM_FORMAT_FLOAT64_BE), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_IEC958_SUBFRAME_LE",
          Napi::Number::New(env, SND_PCM_FORMAT_IEC958_SUBFRAME_LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_IEC958_SUBFRAME_BE",
          Napi::Number::New(env, SND_PCM_FORMAT_IEC958_SUBFRAME_BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_MU_LAW", Napi::Number::New(env, SND_PCM_FORMAT_MU_LAW),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_A_LAW", Napi::Number::New(env, SND_PCM_FORMAT_A_LAW),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_IMA_ADPCM", Napi::Number::New(env, SND_PCM_FORMAT_IMA_ADPCM),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_MPEG", Napi::Number::New(env, SND_PCM_FORMAT_MPEG),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_GSM", Napi::Number::New(env, SND_PCM_FORMAT_GSM),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S20_LE", Napi::Number::New(env, SND_PCM_FORMAT_S20_LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S20_BE", Napi::Number::New(env, SND_PCM_FORMAT_S20_BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U20_LE", Napi::Number::New(env, SND_PCM_FORMAT_U20_LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U20_BE", Napi::Number::New(env, SND_PCM_FORMAT_U20_BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_SPECIAL", Napi::Number::New(env, SND_PCM_FORMAT_SPECIAL),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S24_3LE", Napi::Number::New(env, SND_PCM_FORMAT_S24_3LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S24_3BE", Napi::Number::New(env, SND_PCM_FORMAT_S24_3BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U24_3LE", Napi::Number::New(env, SND_PCM_FORMAT_U24_3LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U24_3BE", Napi::Number::New(env, SND_PCM_FORMAT_U24_3BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S20_3LE", Napi::Number::New(env, SND_PCM_FORMAT_S20_3LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S20_3BE", Napi::Number::New(env, SND_PCM_FORMAT_S20_3BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U20_3LE", Napi::Number::New(env, SND_PCM_FORMAT_U20_3LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U20_3BE", Napi::Number::New(env, SND_PCM_FORMAT_U20_3BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S18_3LE", Napi::Number::New(env, SND_PCM_FORMAT_S18_3LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S18_3BE", Napi::Number::New(env, SND_PCM_FORMAT_S18_3BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U18_3LE", Napi::Number::New(env, SND_PCM_FORMAT_U18_3LE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U18_3BE", Napi::Number::New(env, SND_PCM_FORMAT_U18_3BE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_G723_24", Napi::Number::New(env, SND_PCM_FORMAT_G723_24),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_G723_24_1B",
          Napi::Number::New(env, SND_PCM_FORMAT_G723_24_1B), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_G723_40", Napi::Number::New(env, SND_PCM_FORMAT_G723_40),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_G723_40_1B",
          Napi::Number::New(env, SND_PCM_FORMAT_G723_40_1B), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_DSD_U8", Napi::Number::New(env, SND_PCM_FORMAT_DSD_U8),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_DSD_U16_LE",
          Napi::Number::New(env, SND_PCM_FORMAT_DSD_U16_LE), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_DSD_U32_LE",
          Napi::Number::New(env, SND_PCM_FORMAT_DSD_U32_LE), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_DSD_U16_BE",
          Napi::Number::New(env, SND_PCM_FORMAT_DSD_U16_BE), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_DSD_U32_BE",
          Napi::Number::New(env, SND_PCM_FORMAT_DSD_U32_BE), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_LAST", Napi::Number::New(env, SND_PCM_FORMAT_LAST),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S16", Napi::Number::New(env, SND_PCM_FORMAT_S16),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U16", Napi::Number::New(env, SND_PCM_FORMAT_U16),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S24", Napi::Number::New(env, SND_PCM_FORMAT_S24),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U24", Napi::Number::New(env, SND_PCM_FORMAT_U24),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S32", Napi::Number::New(env, SND_PCM_FORMAT_S32),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U32", Napi::Number::New(env, SND_PCM_FORMAT_U32),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_FLOAT", Napi::Number::New(env, SND_PCM_FORMAT_FLOAT),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_FLOAT64", Napi::Number::New(env, SND_PCM_FORMAT_FLOAT64),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_IEC958_SUBFRAME",
          Napi::Number::New(env, SND_PCM_FORMAT_IEC958_SUBFRAME),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_S20", Napi::Number::New(env, SND_PCM_FORMAT_S20),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "FORMAT_U20", Napi::Number::New(env, SND_PCM_FORMAT_U20),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "SUBFORMAT_STD", Napi::Number::New(env, SND_PCM_SUBFORMAT_STD),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "SUBFORMAT_LAST", Napi::Number::New(env, SND_PCM_SUBFORMAT_LAST),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_OPEN", Napi::Number::New(env, SND_PCM_STATE_OPEN),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_SETUP", Napi::Number::New(env, SND_PCM_STATE_SETUP),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_PREPARED", Napi::Number::New(env, SND_PCM_STATE_PREPARED),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_RUNNING", Napi::Number::New(env, SND_PCM_STATE_RUNNING),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_XRUN", Napi::Number::New(env, SND_PCM_STATE_XRUN),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_DRAINING", Napi::Number::New(env, SND_PCM_STATE_DRAINING),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_PAUSED", Napi::Number::New(env, SND_PCM_STATE_PAUSED),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_SUSPENDED", Napi::Number::New(env, SND_PCM_STATE_SUSPENDED),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_DISCONNECTED",
          Napi::Number::New(env, SND_PCM_STATE_DISCONNECTED), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_LAST", Napi::Number::New(env, SND_PCM_STATE_LAST),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "STATE_PRIVATE1", Napi::Number::New(env, SND_PCM_STATE_PRIVATE1),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "START_DATA", Napi::Number::New(env, SND_PCM_START_DATA),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "START_EXPLICIT", Napi::Number::New(env, SND_PCM_START_EXPLICIT),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "START_LAST", Napi::Number::New(env, SND_PCM_START_LAST),
          napi_enumerable),
      Napi::PropertyDescriptor::Value("XRUN_NONE",
                                      Napi::Number::New(env, SND_PCM_XRUN_NONE),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value("XRUN_STOP",
                                      Napi::Number::New(env, SND_PCM_XRUN_STOP),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value("XRUN_LAST",
                                      Napi::Number::New(env, SND_PCM_XRUN_LAST),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TSTAMP_NONE", Napi::Number::New(env, SND_PCM_TSTAMP_NONE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TSTAMP_ENABLE", Napi::Number::New(env, SND_PCM_TSTAMP_ENABLE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TSTAMP_MMAP", Napi::Number::New(env, SND_PCM_TSTAMP_MMAP),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TSTAMP_LAST", Napi::Number::New(env, SND_PCM_TSTAMP_LAST),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TSTAMP_TYPE_GETTIMEOFDAY",
          Napi::Number::New(env, SND_PCM_TSTAMP_TYPE_GETTIMEOFDAY),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TSTAMP_TYPE_MONOTONIC",
          Napi::Number::New(env, SND_PCM_TSTAMP_TYPE_MONOTONIC),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TSTAMP_TYPE_MONOTONIC_RAW",
          Napi::Number::New(env, SND_PCM_TSTAMP_TYPE_MONOTONIC_RAW),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TSTAMP_TYPE_LAST", Napi::Number::New(env, SND_PCM_TSTAMP_TYPE_LAST),
          napi_enumerable),
      Napi::PropertyDescriptor::Value("NONBLOCK",
                                      Napi::Number::New(env, SND_PCM_NONBLOCK),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "ASYNC", Napi::Number::New(env, SND_PCM_ASYNC), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "ABORT", Napi::Number::New(env, SND_PCM_ABORT), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "NO_AUTO_RESAMPLE", Napi::Number::New(env, SND_PCM_NO_AUTO_RESAMPLE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "NO_AUTO_CHANNELS", Napi::Number::New(env, SND_PCM_NO_AUTO_CHANNELS),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "NO_AUTO_FORMAT", Napi::Number::New(env, SND_PCM_NO_AUTO_FORMAT),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "NO_SOFTVOL", Napi::Number::New(env, SND_PCM_NO_SOFTVOL),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_HW", Napi::Number::New(env, SND_PCM_TYPE_HW), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_HOOKS", Napi::Number::New(env, SND_PCM_TYPE_HOOKS),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_MULTI", Napi::Number::New(env, SND_PCM_TYPE_MULTI),
          napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_FILE",
                                      Napi::Number::New(env, SND_PCM_TYPE_FILE),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_NULL",
                                      Napi::Number::New(env, SND_PCM_TYPE_NULL),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_SHM",
                                      Napi::Number::New(env, SND_PCM_TYPE_SHM),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_INET",
                                      Napi::Number::New(env, SND_PCM_TYPE_INET),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_COPY",
                                      Napi::Number::New(env, SND_PCM_TYPE_COPY),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_LINEAR", Napi::Number::New(env, SND_PCM_TYPE_LINEAR),
          napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_ALAW",
                                      Napi::Number::New(env, SND_PCM_TYPE_ALAW),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_MULAW", Napi::Number::New(env, SND_PCM_TYPE_MULAW),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_ADPCM", Napi::Number::New(env, SND_PCM_TYPE_ADPCM),
          napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_RATE",
                                      Napi::Number::New(env, SND_PCM_TYPE_RATE),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_ROUTE", Napi::Number::New(env, SND_PCM_TYPE_ROUTE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_PLUG",
                                      Napi::Number::New(env, SND_PCM_TYPE_PLUG),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_SHARE", Napi::Number::New(env, SND_PCM_TYPE_SHARE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_METER", Napi::Number::New(env, SND_PCM_TYPE_METER),
          napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_MIX",
                                      Napi::Number::New(env, SND_PCM_TYPE_MIX),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_DROUTE", Napi::Number::New(env, SND_PCM_TYPE_DROUTE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_LBSERVER", Napi::Number::New(env, SND_PCM_TYPE_LBSERVER),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_LINEAR_FLOAT",
          Napi::Number::New(env, SND_PCM_TYPE_LINEAR_FLOAT), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_LADSPA", Napi::Number::New(env, SND_PCM_TYPE_LADSPA),
          napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_DMIX",
                                      Napi::Number::New(env, SND_PCM_TYPE_DMIX),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_JACK",
                                      Napi::Number::New(env, SND_PCM_TYPE_JACK),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_DSNOOP", Napi::Number::New(env, SND_PCM_TYPE_DSNOOP),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_DSHARE", Napi::Number::New(env, SND_PCM_TYPE_DSHARE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_IEC958", Napi::Number::New(env, SND_PCM_TYPE_IEC958),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_SOFTVOL", Napi::Number::New(env, SND_PCM_TYPE_SOFTVOL),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_IOPLUG", Napi::Number::New(env, SND_PCM_TYPE_IOPLUG),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_EXTPLUG", Napi::Number::New(env, SND_PCM_TYPE_EXTPLUG),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "TYPE_MMAP_EMUL", Napi::Number::New(env, SND_PCM_TYPE_MMAP_EMUL),
          napi_enumerable),
      Napi::PropertyDescriptor::Value("TYPE_LAST",
                                      Napi::Number::New(env, SND_PCM_TYPE_LAST),
                                      napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "HOOK_TYPE_HW_PARAMS",
          Napi::Number::New(env, SND_PCM_HOOK_TYPE_HW_PARAMS), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "HOOK_TYPE_HW_FREE",
          Napi::Number::New(env, SND_PCM_HOOK_TYPE_HW_FREE), napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "HOOK_TYPE_CLOSE", Napi::Number::New(env, SND_PCM_HOOK_TYPE_CLOSE),
          napi_enumerable),
      Napi::PropertyDescriptor::Value(
          "HOOK_TYPE_LAST", Napi::Number::New(env, SND_PCM_HOOK_TYPE_LAST),
          napi_enumerable),
  });

  exports.Set(Napi::String::New(env, "constants"), constants);
  return exports;
}

}  // namespace Constants
}  // namespace nodealsa
