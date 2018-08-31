import bindings from 'bindings';
import * as constants from './constants';
export const DEFAULT_OPTIONS = {
  device: 'default',
  stream: constants.STREAM_PLAYBACK,
  format: constants.FORMAT_S16_LE,
  access: constants.ACCESS_RW_INTERLEAVED,
  channels: 2,
  rate: 44100,
  resample: false,
  latency: 500000
  /*::
  class NativePcm {
    constructor(options: Options) {}
    async read(size: number, reset: boolean): Promise<Buffer> {
      return Buffer.alloc(0)
    }
    async write(data: Buffer, reset: boolean): Promise<number> {
      return 0
    }
  }
  */

};
const Native = bindings('nodealsa.node').Pcm;
export default class Pcm extends Native {
  constructor(options = {}) {
    // $FlowFixMe:
    super({ ...DEFAULT_OPTIONS,
      ...options
    });
  }

}