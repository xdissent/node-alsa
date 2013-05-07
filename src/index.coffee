stream = require './stream'
exports.Pcm = require('bindings')('alsa').Pcm
exports.Capture = stream.Capture
exports.Playback = stream.Playback
exports[k] = v for k, v of require './constants'