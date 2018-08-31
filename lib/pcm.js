"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.default = exports.DEFAULT_OPTIONS = void 0;

var _bindings = _interopRequireDefault(require("bindings"));

var constants = _interopRequireWildcard(require("./constants"));

function _interopRequireWildcard(obj) { if (obj && obj.__esModule) { return obj; } else { var newObj = {}; if (obj != null) { for (var key in obj) { if (Object.prototype.hasOwnProperty.call(obj, key)) { var desc = Object.defineProperty && Object.getOwnPropertyDescriptor ? Object.getOwnPropertyDescriptor(obj, key) : {}; if (desc.get || desc.set) { Object.defineProperty(newObj, key, desc); } else { newObj[key] = obj[key]; } } } } newObj.default = obj; return newObj; } }

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

const DEFAULT_OPTIONS = {
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
exports.DEFAULT_OPTIONS = DEFAULT_OPTIONS;
const Native = (0, _bindings.default)('nodealsa.node').Pcm;

class Pcm extends Native {
  constructor(options = {}) {
    // $FlowFixMe:
    super({ ...DEFAULT_OPTIONS,
      ...options
    });
  }

}

exports.default = Pcm;