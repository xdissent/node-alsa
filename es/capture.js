function _defineProperty(obj, key, value) { if (key in obj) { Object.defineProperty(obj, key, { value: value, enumerable: true, configurable: true, writable: true }); } else { obj[key] = value; } return obj; }

import { Readable } from 'stream';
import debug from 'debug';
import Pcm from './pcm';
import { STREAM_CAPTURE } from './constants';
const dbg = debug('alsa:capture');
export default class Capture extends Readable {
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
    this.pcm = new Pcm({ ...pcmOpts,
      stream: STREAM_CAPTURE
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