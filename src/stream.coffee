stream = require 'stream'
alsa = require './constants'
Pcm = require('bindings')('alsa').Pcm
# debug = require('debug') 'alsa:capture'

class Capture extends stream.Readable
  constructor: (@device = 'default', channels = 2, rate = 44100, format = alsa.PCM_FORMAT_S16_LE, access = alsa.PCM_ACCESS_RW_INTERLEAVED, latency = 500) ->
    stream.Readable.call this
    @pcm = new Pcm channels, rate, format, access, (latency * 1000)

  _read: ->
    return false if @pcm.opened
    @pcm.open @device, alsa.PCM_STREAM_CAPTURE, (err) =>
      if err? then throw err else @pcm.read (err, frames) =>
          if err? then throw err else @pcm.close() unless @push frames

class Playback extends stream.Writable
  constructor: (@device = 'default', channels = 2, rate = 44100, format = alsa.PCM_FORMAT_S16_LE, access = alsa.PCM_ACCESS_RW_INTERLEAVED, latency = 500) ->
    stream.Writable.call this
  #   @pcm = new Pcm channels, rate, format, access, (latency * 1000)
  #   @on 'finish', => @pcm.close()

  # _write: (chunk, encoding, callback) ->
  #   return @pcm.write chunk, callback if @pcm.opened
  #   @pcm.open @device, alsa.PCM_STREAM_PLAYBACK, (err) =>
  #     if err? then throw err else @pcm.write chunk, callback

exports.Capture = Capture
exports.Playback = Playback