function _defineProperty(obj, key, value) { if (key in obj) { Object.defineProperty(obj, key, { value: value, enumerable: true, configurable: true, writable: true }); } else { obj[key] = value; } return obj; }

import { Writable } from 'stream';
import debug from 'debug';
import Pcm from './pcm';
import { STREAM_PLAYBACK } from './constants';
const dbg = debug('alsa:playback');
export default class Playback extends Writable {
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
    this.pcm = new Pcm({ ...pcmOpts,
      stream: STREAM_PLAYBACK
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