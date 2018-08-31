"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.default = void 0;

var _stream = require("stream");

var _debug = _interopRequireDefault(require("debug"));

var _pcm = _interopRequireDefault(require("./pcm"));

var _constants = require("./constants");

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _defineProperty(obj, key, value) { if (key in obj) { Object.defineProperty(obj, key, { value: value, enumerable: true, configurable: true, writable: true }); } else { obj[key] = value; } return obj; }

const dbg = (0, _debug.default)('alsa:playback');

class Playback extends _stream.Writable {
  constructor(options = {}) {
    const {
      highWaterMark,
      ...pcmOpts
    } = options;
    super({
      highWaterMark
    });

    _defineProperty(this, "pcm", void 0);

    _defineProperty(this, "_reset", true);

    _defineProperty(this, "remainder", void 0);

    dbg('creating pcm');
    this.pcm = new _pcm.default({ ...pcmOpts,
      stream: _constants.STREAM_PLAYBACK
    });
  }

  async _write(chunk, encoding, callback // $FlowFixMe: Thinks _write has to return a boolean?
  ) {
    try {
      if (!(chunk instanceof Buffer)) throw new Error('Invalid chunk');
      dbg('writing %d bytes', chunk.length);
      const remainder = this.remainder;

      if (remainder) {
        chunk = Buffer.concat([remainder, chunk]);
        dbg('added %d bytes (%d total)', remainder.length, chunk.length);
        delete this.remainder;
      }

      const wrote = await this.pcm.write(chunk, this._reset);
      this._reset = false;
      dbg('wrote %d bytes', wrote);

      if (chunk.length < wrote) {
        dbg('saving %d bytes', chunk.length - wrote);
        this.remainder = chunk.slice(wrote, chunk.length);
      }

      return callback();
    } catch (err) {
      dbg('write err %o', err);
      delete this.remainder;
      this._reset = true;
      return callback(err);
    }
  }

}

exports.default = Playback;