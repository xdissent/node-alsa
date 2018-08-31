// @flow

import {Readable} from 'stream'
import debug from 'debug'
import Pcm, {type Options as PcmOptions} from './pcm'
import {STREAM_CAPTURE} from './constants'

const dbg = debug('alsa:capture')

export type Options = {
  ...PcmOptions
} & {highWaterMark?: number}

export default class Capture extends Readable {
  pcm: Pcm
  _reset: boolean = true

  constructor(options: Options = {}) {
    const {highWaterMark, ...pcmOpts} = options
    super({highWaterMark})
    dbg('creating pcm')
    this.pcm = new Pcm({...pcmOpts, stream: STREAM_CAPTURE})
  }

  async _read(size: number): Promise<void> {
    try {
      dbg('reading %d bytes', size)
      const chunk = await this.pcm.read(size, this._reset)
      this._reset = false
      dbg('read %d bytes', chunk.length)
      const pushed = this.push(chunk)
      if (!pushed) dbg('backpressure detected')
    } catch (err) {
      dbg('read error %o', err)
      this.emit('error', err)
      this._reset = true
    }
  }
}
