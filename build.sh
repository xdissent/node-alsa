#!/usr/bin/env bash

set -e

build() {
  BABEL_ENV="$1" babel --out-dir "$1" --ignore '**/*.test.js,**/*.flow.js' --verbose "src"
  find "src" -name '*.js' ! -name '*.test.js' ! -name '*.flow.js' -print0 \
    | xargs -n 1 -0 bash -c \
      'OUT="${1/src/$0}.flow"; cp -f "$1" "$OUT" && echo "$1 -> $OUT"' "$1" | awk '{print}END{print "Successfully copied",NR,NR==1?"file":"files","with Flow."}'
}

for e in lib es; do build $e; done
node-gyp build --debug
