// @flow

import {Writable} from 'stream'
import debug from 'debug'
import Pcm, {type Options as PcmOptions} from './pcm'
import {STREAM_PLAYBACK} from './constants'

const dbg = debug('alsa:playback')

export type Options = {
  ...PcmOptions
} & {highWaterMark?: number}

export default class Playback extends Writable {
  pcm: Pcm
  _reset: boolean = true
  remainder: ?Buffer

  constructor(options: Options = {}) {
    const {highWaterMark, ...pcmOpts} = options
    super({highWaterMark})
    dbg('creating pcm')
    this.pcm = new Pcm({...pcmOpts, stream: STREAM_PLAYBACK})
  }

  async _write(
    chunk: Buffer | string,
    encoding: string,
    callback: (err?: Error) => void
    // $FlowFixMe: Thinks _write has to return a boolean?
  ): Promise<void> {
    try {
      if (!(chunk instanceof Buffer)) throw new Error('Invalid chunk')
      dbg('writing %d bytes', chunk.length)
      const remainder = this.remainder
      if (remainder) {
        chunk = Buffer.concat([remainder, chunk])
        dbg('added %d bytes (%d total)', remainder.length, chunk.length)
        delete this.remainder
      }
      const wrote = await this.pcm.write(chunk, this._reset)
      this._reset = false
      dbg('wrote %d bytes', wrote)
      if (chunk.length < wrote) {
        dbg('saving %d bytes', chunk.length - wrote)
        this.remainder = chunk.slice(wrote, chunk.length)
      }
      return callback()
    } catch (err) {
      dbg('write err %o', err)
      delete this.remainder
      this._reset = true
      return callback(err)
    }
  }
}
