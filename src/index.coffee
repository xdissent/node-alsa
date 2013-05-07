stream = require './stream'
exports.Pcm = require('../build/Release/alsa').Pcm
exports.Capture = stream.Capture
exports.Playback = stream.Playback
exports[k] = v for k, v of require './constants'