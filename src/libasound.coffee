ffi = require 'ffi'
ref = require 'ref'
stream = require 'stream'

intPtr = ref.refType 'int'
uintPtr = ref.refType 'uint'
ulongPtr = ref.refType 'ulong'
voidPtr = ref.refType 'void'
voidPtrPtr = ref.refType voidPtr

module.exports = libasound = ffi.Library 'libasound',
  # const char *snd_strerror(int errnum)
  snd_strerror: ['string', ['int']]

  # int snd_pcm_open(snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode);
  snd_pcm_open: ['int', [voidPtrPtr, 'string', 'int', 'int']]

  # int snd_pcm_start(snd_pcm_t *pcm);
  snd_pcm_start: ['int', [voidPtr]]

  # int snd_pcm_drop(snd_pcm_t *pcm);
  snd_pcm_drop: ['int', [voidPtr]]

  # int snd_pcm_drain(snd_pcm_t *pcm);
  snd_pcm_drain: ['int', [voidPtr]]

  # int snd_pcm_close(snd_pcm_t *pcm);
  snd_pcm_close: ['int', [voidPtr]]

  # int snd_pcm_prepare(snd_pcm_t *pcm);
  snd_pcm_prepare: ['int', [voidPtr]]

  # int snd_pcm_reset(snd_pcm_t *pcm);
  snd_pcm_reset: ['int', [voidPtr]]

  # const char *snd_pcm_name(snd_pcm_t *pcm);
  snd_pcm_name: ['string', [voidPtr]]

  # snd_pcm_state_t snd_pcm_state(snd_pcm_t *pcm);
  snd_pcm_state: ['int', [voidPtr]]

  # const char *snd_pcm_state_name(const snd_pcm_state_t state);
  snd_pcm_state_name: ['string', ['int']]

  # size_t snd_pcm_hw_params_sizeof(void);
  snd_pcm_hw_params_sizeof: ['int', []]

  # int snd_pcm_hw_params_any(snd_pcm_t *pcm, snd_pcm_hw_params_t *params);
  snd_pcm_hw_params_any: ['int', [voidPtr, voidPtr]]

  # int snd_pcm_hw_params_set_access(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, snd_pcm_access_t _access);
  snd_pcm_hw_params_set_access: ['int', [voidPtr, voidPtr, 'int']]

  # int snd_pcm_hw_params_get_access(const snd_pcm_hw_params_t *params, snd_pcm_access_t *_access);
  snd_pcm_hw_params_get_access: ['int', [voidPtr, intPtr]]

  # int snd_pcm_hw_params_set_format(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, snd_pcm_format_t val);
  snd_pcm_hw_params_set_format: ['int', [voidPtr, voidPtr, 'int']]

  # int snd_pcm_hw_params_get_format(const snd_pcm_hw_params_t *params, snd_pcm_format_t *val);
  snd_pcm_hw_params_get_format: ['int', [voidPtr, intPtr]]

  # int snd_pcm_hw_params_set_channels(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int val);
  snd_pcm_hw_params_set_channels: ['int', [voidPtr, voidPtr, 'uint']]  

  # int snd_pcm_hw_params_get_channels(const snd_pcm_hw_params_t *params, unsigned int *val);
  snd_pcm_hw_params_get_channels: ['int', [voidPtr, uintPtr]]

  # int snd_pcm_hw_params_set_rate_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
  snd_pcm_hw_params_set_rate_near: ['int', [voidPtr, voidPtr, uintPtr, intPtr]]

  # int snd_pcm_hw_params_set_rate(snd_pcm_t *pcm, snd_pcm_hw_params_t *params, unsigned int val, int dir);
  snd_pcm_hw_params_set_rate: ['int', [voidPtr, voidPtr, 'uint', 'int']]

  # int snd_pcm_hw_params_get_rate(const snd_pcm_hw_params_t *params, unsigned int *val, int *dir);
  snd_pcm_hw_params_get_rate: ['int', [voidPtr, uintPtr, intPtr]]

  # int snd_pcm_hw_params(snd_pcm_t *pcm, snd_pcm_hw_params_t *params);
  snd_pcm_hw_params: ['int', [voidPtr, voidPtr]]

  # int snd_pcm_hw_params_get_period_size(const snd_pcm_hw_params_t *params, snd_pcm_uframes_t *frames, int *dir);
  snd_pcm_hw_params_get_period_size: ['int', [voidPtr, ulongPtr, intPtr]]

  # snd_pcm_sframes_t snd_pcm_readi(snd_pcm_t *pcm, void *buffer, snd_pcm_uframes_t size);
  snd_pcm_readi: ['long', [voidPtr, voidPtr, 'ulong']]

  # snd_pcm_sframes_t snd_pcm_writei(snd_pcm_t *pcm, const void *buffer, snd_pcm_uframes_t size);
  snd_pcm_writei: ['long', [voidPtr, voidPtr, 'ulong']]

  # ssize_t snd_pcm_format_size(snd_pcm_format_t format, size_t samples);
  snd_pcm_format_size: ['int', ['int', 'int']]

  # snd_pcm_sframes_t snd_pcm_avail_update(snd_pcm_t *pcm);
  snd_pcm_avail_update: ['long', [voidPtr]]

# snd_pcm_hw_params_t
libasound.pcmHwParamsPtr = 
  name: 'snd_pcm_hw_params'
  size: libasound.snd_pcm_hw_params_sizeof()
  alignment: 1
  indirection: 1

# snd_pcm_t
libasound.pcmPtr = voidPtr
libasound.pcmPtrPtr = voidPtrPtr

# Load constants
libasound[k] = v for k, v of require './constants'
