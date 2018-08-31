import bindings from 'bindings';
const {
  constants
} = bindings('nodealsa.node');
export const CLASS_GENERIC = constants.CLASS_GENERIC;
export const CLASS_MULTI = constants.CLASS_MULTI;
export const CLASS_MODEM = constants.CLASS_MODEM;
export const CLASS_DIGITIZER = constants.CLASS_DIGITIZER;
export const CLASS_LAST = constants.CLASS_LAST;
export const SUBCLASS_GENERIC_MIX = constants.SUBCLASS_GENERIC_MIX;
export const SUBCLASS_MULTI_MIX = constants.SUBCLASS_MULTI_MIX;
export const SUBCLASS_LAST = constants.SUBCLASS_LAST;
export const STREAM_PLAYBACK = constants.STREAM_PLAYBACK;
export const STREAM_CAPTURE = constants.STREAM_CAPTURE;
export const STREAM_LAST = constants.STREAM_LAST;
export const ACCESS_MMAP_INTERLEAVED = constants.ACCESS_MMAP_INTERLEAVED;
export const ACCESS_MMAP_NONINTERLEAVED = constants.ACCESS_MMAP_NONINTERLEAVED;
export const ACCESS_MMAP_COMPLEX = constants.ACCESS_MMAP_COMPLEX;
export const ACCESS_RW_INTERLEAVED = constants.ACCESS_RW_INTERLEAVED;
export const ACCESS_RW_NONINTERLEAVED = constants.ACCESS_RW_NONINTERLEAVED;
export const ACCESS_LAST = constants.ACCESS_LAST;
export const FORMAT_UNKNOWN = constants.FORMAT_UNKNOWN;
export const FORMAT_S8 = constants.FORMAT_S8;
export const FORMAT_U8 = constants.FORMAT_U8;
export const FORMAT_S16_LE = constants.FORMAT_S16_LE;
export const FORMAT_S16_BE = constants.FORMAT_S16_BE;
export const FORMAT_U16_LE = constants.FORMAT_U16_LE;
export const FORMAT_U16_BE = constants.FORMAT_U16_BE;
export const FORMAT_S24_LE = constants.FORMAT_S24_LE;
export const FORMAT_S24_BE = constants.FORMAT_S24_BE;
export const FORMAT_U24_LE = constants.FORMAT_U24_LE;
export const FORMAT_U24_BE = constants.FORMAT_U24_BE;
export const FORMAT_S32_LE = constants.FORMAT_S32_LE;
export const FORMAT_S32_BE = constants.FORMAT_S32_BE;
export const FORMAT_U32_LE = constants.FORMAT_U32_LE;
export const FORMAT_U32_BE = constants.FORMAT_U32_BE;
export const FORMAT_FLOAT_LE = constants.FORMAT_FLOAT_LE;
export const FORMAT_FLOAT_BE = constants.FORMAT_FLOAT_BE;
export const FORMAT_FLOAT64_LE = constants.FORMAT_FLOAT64_LE;
export const FORMAT_FLOAT64_BE = constants.FORMAT_FLOAT64_BE;
export const FORMAT_IEC958_SUBFRAME_LE = constants.FORMAT_IEC958_SUBFRAME_LE;
export const FORMAT_IEC958_SUBFRAME_BE = constants.FORMAT_IEC958_SUBFRAME_BE;
export const FORMAT_MU_LAW = constants.FORMAT_MU_LAW;
export const FORMAT_A_LAW = constants.FORMAT_A_LAW;
export const FORMAT_IMA_ADPCM = constants.FORMAT_IMA_ADPCM;
export const FORMAT_MPEG = constants.FORMAT_MPEG;
export const FORMAT_GSM = constants.FORMAT_GSM;
export const FORMAT_S20_LE = constants.FORMAT_S20_LE;
export const FORMAT_S20_BE = constants.FORMAT_S20_BE;
export const FORMAT_U20_LE = constants.FORMAT_U20_LE;
export const FORMAT_U20_BE = constants.FORMAT_U20_BE;
export const FORMAT_SPECIAL = constants.FORMAT_SPECIAL;
export const FORMAT_S24_3LE = constants.FORMAT_S24_3LE;
export const FORMAT_S24_3BE = constants.FORMAT_S24_3BE;
export const FORMAT_U24_3LE = constants.FORMAT_U24_3LE;
export const FORMAT_U24_3BE = constants.FORMAT_U24_3BE;
export const FORMAT_S20_3LE = constants.FORMAT_S20_3LE;
export const FORMAT_S20_3BE = constants.FORMAT_S20_3BE;
export const FORMAT_U20_3LE = constants.FORMAT_U20_3LE;
export const FORMAT_U20_3BE = constants.FORMAT_U20_3BE;
export const FORMAT_S18_3LE = constants.FORMAT_S18_3LE;
export const FORMAT_S18_3BE = constants.FORMAT_S18_3BE;
export const FORMAT_U18_3LE = constants.FORMAT_U18_3LE;
export const FORMAT_U18_3BE = constants.FORMAT_U18_3BE;
export const FORMAT_G723_24 = constants.FORMAT_G723_24;
export const FORMAT_G723_24_1B = constants.FORMAT_G723_24_1B;
export const FORMAT_G723_40 = constants.FORMAT_G723_40;
export const FORMAT_G723_40_1B = constants.FORMAT_G723_40_1B;
export const FORMAT_DSD_U8 = constants.FORMAT_DSD_U8;
export const FORMAT_DSD_U16_LE = constants.FORMAT_DSD_U16_LE;
export const FORMAT_DSD_U32_LE = constants.FORMAT_DSD_U32_LE;
export const FORMAT_DSD_U16_BE = constants.FORMAT_DSD_U16_BE;
export const FORMAT_DSD_U32_BE = constants.FORMAT_DSD_U32_BE;
export const FORMAT_LAST = constants.FORMAT_LAST;
export const FORMAT_S16 = constants.FORMAT_S16;
export const FORMAT_U16 = constants.FORMAT_U16;
export const FORMAT_S24 = constants.FORMAT_S24;
export const FORMAT_U24 = constants.FORMAT_U24;
export const FORMAT_S32 = constants.FORMAT_S32;
export const FORMAT_U32 = constants.FORMAT_U32;
export const FORMAT_FLOAT = constants.FORMAT_FLOAT;
export const FORMAT_FLOAT64 = constants.FORMAT_FLOAT64;
export const FORMAT_IEC958_SUBFRAME = constants.FORMAT_IEC958_SUBFRAME;
export const FORMAT_S20 = constants.FORMAT_S20;
export const FORMAT_U20 = constants.FORMAT_U20;
export const SUBFORMAT_STD = constants.SUBFORMAT_STD;
export const SUBFORMAT_LAST = constants.SUBFORMAT_LAST;
export const STATE_OPEN = constants.STATE_OPEN;
export const STATE_SETUP = constants.STATE_SETUP;
export const STATE_PREPARED = constants.STATE_PREPARED;
export const STATE_RUNNING = constants.STATE_RUNNING;
export const STATE_XRUN = constants.STATE_XRUN;
export const STATE_DRAINING = constants.STATE_DRAINING;
export const STATE_PAUSED = constants.STATE_PAUSED;
export const STATE_SUSPENDED = constants.STATE_SUSPENDED;
export const STATE_DISCONNECTED = constants.STATE_DISCONNECTED;
export const STATE_LAST = constants.STATE_LAST;
export const STATE_PRIVATE1 = constants.STATE_PRIVATE1;
export const START_DATA = constants.START_DATA;
export const START_EXPLICIT = constants.START_EXPLICIT;
export const START_LAST = constants.START_LAST;
export const XRUN_NONE = constants.XRUN_NONE;
export const XRUN_STOP = constants.XRUN_STOP;
export const XRUN_LAST = constants.XRUN_LAST;
export const TSTAMP_NONE = constants.TSTAMP_NONE;
export const TSTAMP_ENABLE = constants.TSTAMP_ENABLE;
export const TSTAMP_MMAP = constants.TSTAMP_MMAP;
export const TSTAMP_LAST = constants.TSTAMP_LAST;
export const TSTAMP_TYPE_GETTIMEOFDAY = constants.TSTAMP_TYPE_GETTIMEOFDAY;
export const TSTAMP_TYPE_MONOTONIC = constants.TSTAMP_TYPE_MONOTONIC;
export const TSTAMP_TYPE_MONOTONIC_RAW = constants.TSTAMP_TYPE_MONOTONIC_RAW;
export const TSTAMP_TYPE_LAST = constants.TSTAMP_TYPE_LAST;
export const NONBLOCK = constants.NONBLOCK;
export const ASYNC = constants.ASYNC;
export const ABORT = constants.ABORT;
export const NO_AUTO_RESAMPLE = constants.NO_AUTO_RESAMPLE;
export const NO_AUTO_CHANNELS = constants.NO_AUTO_CHANNELS;
export const NO_AUTO_FORMAT = constants.NO_AUTO_FORMAT;
export const NO_SOFTVOL = constants.NO_SOFTVOL;
export const TYPE_HW = constants.TYPE_HW;
export const TYPE_HOOKS = constants.TYPE_HOOKS;
export const TYPE_MULTI = constants.TYPE_MULTI;
export const TYPE_FILE = constants.TYPE_FILE;
export const TYPE_NULL = constants.TYPE_NULL;
export const TYPE_SHM = constants.TYPE_SHM;
export const TYPE_INET = constants.TYPE_INET;
export const TYPE_COPY = constants.TYPE_COPY;
export const TYPE_LINEAR = constants.TYPE_LINEAR;
export const TYPE_ALAW = constants.TYPE_ALAW;
export const TYPE_MULAW = constants.TYPE_MULAW;
export const TYPE_ADPCM = constants.TYPE_ADPCM;
export const TYPE_RATE = constants.TYPE_RATE;
export const TYPE_ROUTE = constants.TYPE_ROUTE;
export const TYPE_PLUG = constants.TYPE_PLUG;
export const TYPE_SHARE = constants.TYPE_SHARE;
export const TYPE_METER = constants.TYPE_METER;
export const TYPE_MIX = constants.TYPE_MIX;
export const TYPE_DROUTE = constants.TYPE_DROUTE;
export const TYPE_LBSERVER = constants.TYPE_LBSERVER;
export const TYPE_LINEAR_FLOAT = constants.TYPE_LINEAR_FLOAT;
export const TYPE_LADSPA = constants.TYPE_LADSPA;
export const TYPE_DMIX = constants.TYPE_DMIX;
export const TYPE_JACK = constants.TYPE_JACK;
export const TYPE_DSNOOP = constants.TYPE_DSNOOP;
export const TYPE_DSHARE = constants.TYPE_DSHARE;
export const TYPE_IEC958 = constants.TYPE_IEC958;
export const TYPE_SOFTVOL = constants.TYPE_SOFTVOL;
export const TYPE_IOPLUG = constants.TYPE_IOPLUG;
export const TYPE_EXTPLUG = constants.TYPE_EXTPLUG;
export const TYPE_MMAP_EMUL = constants.TYPE_MMAP_EMUL;
export const TYPE_LAST = constants.TYPE_LAST;
export const HOOK_TYPE_HW_PARAMS = constants.HOOK_TYPE_HW_PARAMS;
export const HOOK_TYPE_HW_FREE = constants.HOOK_TYPE_HW_FREE;
export const HOOK_TYPE_CLOSE = constants.HOOK_TYPE_CLOSE;
export const HOOK_TYPE_LAST = constants.HOOK_TYPE_LAST;