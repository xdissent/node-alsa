stream = require 'stream'
alsa = require './constants'
Pcm = require('../build/Release/alsa').Pcm

class Capture extends stream.Readable
  constructor: (@device = 'default', channels = 2, rate = 44100, format = alsa.FORMAT_S16_LE, access = alsa.ACCESS_RW_INTERLEAVED, latency = 500) ->
    stream.Readable.call this
    @pcm = new Pcm channels, rate, format, access, (latency * 1000)

  _read: ->
    return false if @pcm.opened
    @pcm.open @device, alsa.STREAM_CAPTURE, (err) =>
      if err? then throw err else @pcm.read (err, frames) =>
          if err? then throw err else @pcm.close() unless @push frames

class Playback extends stream.Writable
  constructor: (@device = 'default', channels = 2, rate = 44100, format = alsa.FORMAT_S16_LE, access = alsa.ACCESS_RW_INTERLEAVED, latency = 500) ->
    stream.Writable.call this
    @pcm = new Pcm channels, rate, format, access, (latency * 1000)
    @on 'unpipe', => @pcm.close() if @pcm.opened

  _write: (chunk, encoding, callback) ->
    return @pcm.write chunk, callback if @pcm.opened
    @pcm.open @device, alsa.STREAM_PLAYBACK, (err) => 
      if err? then throw err else @pcm.write chunk, callback

exports.Capture = Capture
exports.Playback = Playback