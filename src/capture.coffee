stream = require 'stream'
Pcm = require './pcm'
libasound = require './libasound'

class Capture extends stream.Readable
  constructor: (device = 'default', channels = 2, rate = 44100, format = libasound.PCM_FORMAT_S16_LE, access = libasound.PCM_ACCESS_RW_INTERLEAVED, @async = false) ->
    stream.Readable.call this

    @pcm = new Pcm device, libasound.PCM_STREAM_CAPTURE
    @pcm.open().access(access).format(format).channels(channels).rate(rate).prepare()
    @on 'end', => @pcm.drop().drain().close()

    @periodsPerBuffer = if @async then 32 else 4 # Async really sucks!
    @framesPerPeriod = @pcm.framesPerPeriod()
    @bytesPerFrame = @pcm.bytesPerFrame()
    @framesPerBuffer = @framesPerPeriod * @periodsPerBuffer
    @bytesPerBuffer = @framesPerBuffer * @bytesPerFrame
    @buffer = new Buffer @bytesPerBuffer

  async_read: (size) ->
    @pcm.readi @buffer, @framesPerBuffer, (err, readFrames) =>
      return @push '' if readFrames < 0
      return @push null if readFrames == 0
      @push @buffer.slice(0, readFrames * @bytesPerFrame)

  sync_read: (size) ->
    readFrames = @pcm.readi @buffer, @framesPerBuffer
    return @push '' if readFrames < 0
    return @push null if readFrames == 0
    @push @buffer.slice(0, readFrames * @bytesPerFrame)

  _read: (size) -> if @async then @async_read size else @sync_read size

module.exports = Capture