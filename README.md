node-alsa
=========

ALSA bindings for Node.js


Requirements
------------

You must have a system on which ALSA is supported with the `libasound2-dev` package installed:

```sh
$ sudo apt-get install libasound2-dev
```

You'll also probably want to have an audio device for either playback or capture.


Installation
------------

Install with npm:

```sh
$ npm install alsa
```

or via git:

```sh
$ npm install git+https://github.com/xdissent/node-alsa.git
```


Usage
-----

The `alsa` module exports 2 high-level classes for playback and recording, a simple wrapper class around PCM operations, and a subset of the low-level libasound2 functions.

```js
var alsa = require('alsa'),

  // The following variables represent the defaults for the Playback and Capture constructors.
  device = 'default',                                 // ALSA default device
  channels = 2,                                       // Stereo
  rate = 44100,                                       // Sample rate
  format = alsa.libasound.PCM_FORMAT_S16_LE,          // PCM format (signed 16 bit LE int)
  access = alsa.libasound.PCM_ACCESS_RW_INTERLEAVED,  // Access mode
  async =  false;                                     // Async-mode - YOU DO NOT WANT THIS!

// The Capture class is a stream.Readable subclass.
var capture = new alsa.Capture(device, channels, rate, format, access, async);
capture.pipe(process.stdout);   // Treat it like any other readable stream.

// The Playback class is a stream.Writable subclass.
var playback = new alsa.Playback(device, channels, rate, format, access, async);
process.stdin.pipe(playback);   // Treat it like any other writable stream.

// The Pcm class is a wrapper around common libasound2 functions.
var pcm = new alsa.Pcm(device, alsa.libasound.PCM_STREAM_CAPTURE);  // or PCM_STREAM_PLAYBACK

// Open PCM device, set parameters, and prepare (chainable).
pcm.open().access(access).format(format).channels(channels).rate(rate).prepare();

// PCM parameters are combo getters and setters.
console.log('Channels: ' + pcm.channels());

// Calculate some sizes and create a buffer.
var bytesPerFrame = pcm.bytesPerFrame(),
  framesPerBuffer = 1024,
  bufferSize = framesPerBuffer * bytesPerFrame,
  buffer = new Buffer(bufferSize);

// Read frames from PCM device into the buffer, then get the raw frames.
var readFrames = pcm.readi(buffer, framesPerBuffer),
  frames = buffer.slice(0, readFrames * bytesPerFrame);
  
// Low level libasound2 api access works too (pcm.pcm is a libasound2 snd_pcm_t pointer).
console.log(alsa.libasound.snd_pcm_name(pcm.pcm));
```


Examples
--------

Downsample audio captured from an ALSA device to a sample rate of 11025, in mono, encoded to 32 bit float PCM data and output to stdout:

```coffeescript
alsa = require 'alsa'
stream = require 'stream'

class Downer extends stream.Transform
  constructor: -> stream.Transform.call this

  # Naively drop 3 out of 4 samples, combine the interleaved left and right into 
  # a single sample, then convert to 32 bit float PCM data.
  _transform: (chunk, encoding, callback) ->
  
    # This is the naive part. Should be checking for partial samples.
    floats = new Buffer chunk.length / 4
    f = 0
    for i in [0...chunk.length] by 16 # 4 bytes per frame downsampled by 4.
      l16 = chunk.readInt16LE i       # Read left channel sample.
      r16 = chunk.readInt16LE i + 2   # Read right channel sample.
      # Sum samples and normalize as a float
      floats.writeFloatLE (l16 + r16) / 65536, f
      f += 4                          # Skip ahead 4 bytes in buffer (32 bit float).
    @push floats  # Write transformed samples to stream...
    callback()    # And we're done.

# Create a Capture and a Downer and chain them to stdout.
capture = new alsa.Capture
downer = new Downer
capture.pipe(downer).pipe(process.stdout)
```