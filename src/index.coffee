exports.Pcm = require('bindings')('alsa').Pcm
exports.Capture = require './capture'
exports.Playback = require './playback'
exports[k] = v for k, v of require './constants'