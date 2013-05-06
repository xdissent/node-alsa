stream = require 'stream'
alsa = require './constants'
Pcm = require('bindings')('alsa').Pcm
debug = require('debug') 'alsa:capture'

class Capture extends stream.Readable
  constructor: (@device = 'default', channels = 2, rate = 44100, format = alsa.PCM_FORMAT_S16_LE, access = alsa.PCM_ACCESS_RW_INTERLEAVED, latency = 500) ->
    stream.Readable.call this
    @pcm = new Pcm channels, rate, format, access, (latency * 1000)
    @pcm.onData = (frames) =>  @pcm.stopReading() unless @push frames
    @pcm.onEnd = => @pcm.close()

  _read: ->
    @pcm.open(@device, alsa.PCM_STREAM_CAPTURE).startReading() unless @pcm.reading

module.exports = Capture
