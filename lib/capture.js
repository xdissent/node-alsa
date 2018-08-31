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

const dbg = (0, _debug.default)('alsa:capture');

class Capture extends _stream.Readable {
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

    dbg('creating pcm');
    this.pcm = new _pcm.default({ ...pcmOpts,
      stream: _constants.STREAM_CAPTURE
    });
  }

  async _read(size) {
    try {
      dbg('reading %d bytes', size);
      const chunk = await this.pcm.read(size, this._reset);
      this._reset = false;
      dbg('read %d bytes', chunk.length);
      const pushed = this.push(chunk);
      if (!pushed) dbg('backpressure detected');
    } catch (err) {
      dbg('read error %o', err);
      this.emit('error', err);
      this._reset = true;
    }
  }

}

exports.default = Capture;