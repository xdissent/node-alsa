// @flow

const {
  promises: {readFile, writeFile}
} = require('fs')
const {spawn} = require('child_process')
const {resolve, basename} = require('path')
const prettier = require('prettier')

const formatJs = async (name, src) =>
  // $FlowFixMe:
  prettier.format(
    src,
    // $FlowFixMe:
    await prettier.resolveConfig(process.cwd(), {
      config: resolve('.prettierrc')
    })
  )

const formatCpp = (name, src) =>
  new Promise((resolve, reject) => {
    let out = ''
    const child = spawn('clang-format', [
      '-style=google',
      `-assume-filename=${basename(name)}`
    ])
    child.once(
      'close',
      code =>
        code === 0
          ? resolve(out)
          : reject(new Error(`clang-format failed: ${code}`))
    )
    child.stdout.on('data', data => (out += data.toString('utf8')))
    child.stdin.end(src)
  })

const writeJs = async (name, src) =>
  writeFile(name, await formatJs(name, src), 'utf8')

const writeCpp = async (name, src) =>
  writeFile(name, await formatCpp(name, src), 'utf8')

const findConstants = src => {
  const consts = new Set()
  const re = /(SND_PCM_\w+)/g
  while (true) {
    const m = re.exec(src)
    if (!m) break
    consts.add(m[1])
  }
  return [...consts].filter(name => !/DLSYM/.test(name)).map(name => ({name}))
}

const genConstant = c => ({
  exp: `Napi::PropertyDescriptor::Value("${c.name.replace(
    /^SND_PCM_/,
    ''
  )}", Napi::Number::New(env, ${c.name}), napi_enumerable),`,
  flow: `export const ${c.name.replace(
    /^SND_PCM_/,
    ''
  )}: number = constants.${c.name.replace(/^SND_PCM_/, '')}`
})

const nsOpen = `
namespace nodealsa {
namespace Constants {`

const nsClose = `
}  // namespace Constants
}  // namespace nodealsa
`

const includes = `
#include <alsa/asoundlib.h>
#include <napi.h>
`

const cppOpen = `
#include "constants.h"${includes}
${nsOpen}
`

const cppClose = `${nsClose}`

const hppOpen = `
#ifndef NODE_ALSA_CONSTANTS_H_
#define NODE_ALSA_CONSTANTS_H_
${includes}
${nsOpen}

Napi::Object Init(Napi::Env env, Napi::Object exports);
`

const hppClose = `${nsClose}
#endif  // NODE_ALSA_CONSTANTS_H_
`

const initOpen = `Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Napi::Object constants = Napi::Object::New(env);
`

const initClose = `
  exports.Set(Napi::String::New(env, "constants"), constants);
  return exports;
}`

const jsOpen = `// @flow

import bindings from 'bindings'

const {constants} = bindings('nodealsa.node')
`

const constantsOpen = `  constants.DefineProperties({`
const constantsClose = `  });`

const main = async path => {
  const src = (await readFile(resolve(path, 'pcm.h'))).toString('utf8')

  const constants = findConstants(src).map(genConstant)

  const cpp = [
    cppOpen,
    initOpen,
    constantsOpen,
    ...constants.map(c => c.exp),
    constantsClose,
    initClose,
    cppClose
  ].join('\n')

  const hpp = [hppOpen, hppClose].join('\n')

  const js = [jsOpen, ...constants.map(c => c.flow)].join('\n')

  await writeCpp('./src/constants.h', hpp)
  await writeCpp('./src/constants.cc', cpp)
  await writeJs('./src/constants.js', js)
}

main(resolve(process.argv[2] || '.')).then(() => {}, err => console.error(err))
