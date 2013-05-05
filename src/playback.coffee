stream = require 'stream'
Pcm = require './pcm'
libasound = require './libasound'

class Playback extends stream.Writable
  constructor: (device = 'default', channels = 2, rate = 44100, format = libasound.PCM_FORMAT_S16_LE, access = libasound.PCM_ACCESS_RW_INTERLEAVED, @async = false) ->
    stream.Writable.call this

    @pcm = new Pcm device, libasound.PCM_STREAM_PLAYBACK
    @pcm.open().access(access).format(format).channels(channels).rate(rate).prepare()
    @on 'end', => @pcm.drop().drain().close()
    @bytesPerFrame = @pcm.bytesPerFrame()

  async_write: (chunk, encoding, callback) ->
    @pcm.writei chunk, chunk.length / @bytesPerFrame, (err, wroteFrames) =>
      return callback wroteFrames if wroteFrames < 0
      return callback "No frames written" if wroteFrames == 0
      callback()

  sync_write: (chunk, encoding, callback) ->
    wroteFrames = @pcm.writei chunk, chunk.length / @bytesPerFrame
    return callback wroteFrames if wroteFrames < 0
    return callback "No frames written" if wroteFrames == 0
    callback()

  _write: (chunk, encoding, callback) -> 
    if @async
      @async_write chunk, encoding, callback
    else
      @sync_write chunk, encoding, callback

module.exports = Playback