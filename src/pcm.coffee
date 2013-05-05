ref = require 'ref'
libasound = require './libasound'

class Pcm
  constructor: (@device, @stream, @mode = 0) ->

  catchErr: (err, msg) -> throw "ERROR: #{msg} #{libasound.snd_strerror err}" if err < 0; this

  start: -> @catchErr libasound.snd_pcm_start(@pcm), "Can't start pcm"
  prepare: -> @updateHwParams(); @catchErr libasound.snd_pcm_prepare(@pcm), "Can't prepare pcm"
  drain: -> @catchErr libasound.snd_pcm_drain(@pcm), "Can't drain pcm"
  drop: -> @catchErr libasound.snd_pcm_drop(@pcm), "Can't drop pcm"
  close: -> [pcm, @pcm] = [@pcm, null]; @catchErr libasound.snd_pcm_close(pcm), "Can't close pcm"

  name: -> libasound.snd_pcm_name @pcm
  state: -> libasound.snd_pcm_state @pcm
  stateName: -> libasound.snd_pcm_state_name @state()

  open: ->
    pcmPtr = ref.alloc libasound.pcmPtrPtr
    @catchErr libasound.snd_pcm_open(pcmPtr, @device, @stream, @mode), "Can't open \"#{@device}\" PCM device"
    @pcm = pcmPtr.deref()
    this

  hwParams: ->
    @_hwParams ?= @_loadDefaultHwParams()

  _loadDefaultHwParams: ->
    hwParams = ref.alloc libasound.pcmHwParamsPtr
    @catchErr libasound.snd_pcm_hw_params_any @pcm, hwParams
    hwParams

  updateHwParams: -> @catchErr libasound.snd_pcm_hw_params(@pcm, @hwParams()), "Can't set hw params"

  setHwParam: (param, values...) ->
    @catchErr libasound["snd_pcm_hw_params_set_#{param}"](@pcm, @hwParams(), values...), "Can't set #{param}"

  getHwParam: (param, args...) ->
    paramPtr = ref.alloc 'int'
    @catchErr libasound["snd_pcm_hw_params_get_#{param}"](@hwParams(), paramPtr, args...), "Can't get #{param}"
    paramPtr.deref()

  access: (access) -> if access? then @setHwParam 'access', access else @getHwParam 'access'
  format: (format) -> if format? then @setHwParam 'format', format else @getHwParam 'format'
  channels: (channels) -> if channels? then @setHwParam 'channels', channels else @getHwParam 'channels'
  rate: (rate, dir = 0) -> if rate? then @setHwParam 'rate', rate, dir else @getHwParam 'rate', ref.alloc('int')

  framesPerPeriod: ->
    framesPtr = ref.alloc 'ulong'
    dirPtr = ref.alloc 'int'
    @catchErr libasound.snd_pcm_hw_params_get_period_size(@hwParams(), framesPtr, dirPtr), "Can't get frames per period"
    framesPtr.deref()

  bytesPerFrame: -> libasound.snd_pcm_format_size @format(), @channels()

  readi: (buffer, framesPerBuffer, callback) ->
    if callback?
      libasound.snd_pcm_readi.async @pcm, buffer, framesPerBuffer, callback
    else
      libasound.snd_pcm_readi @pcm, buffer, framesPerBuffer

  writei: (buffer, framesPerBuffer, callback) ->
    if callback?
      libasound.snd_pcm_writei.async @pcm, buffer, framesPerBuffer, callback
    else
      libasound.snd_pcm_writei @pcm, buffer, framesPerBuffer

module.exports = Pcm