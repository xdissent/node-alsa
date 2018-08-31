{
  "targets": [
    {
      "target_name": "nodealsa",
      "sources": [
        "src/constants.cc",
        "src/index.cc",
        "src/pcm.cc"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7"
      },
      "msvs_settings": {
        "VCCLCompilerTool": {"ExceptionHandling": 1}
      },
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "defines": ["NAPI_EXPERIMENTAL"],
      "libraries": ["-lasound"]
    }
  ]
}
