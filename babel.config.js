module.exports = {
  plugins: ['@babel/plugin-proposal-class-properties'],
  presets: [
    '@babel/preset-flow',
    [
      '@babel/preset-env',
      {
        targets: process.env.NODE_ENV === 'test' ? {node: true} : {node: '8.6'},
        modules: process.env.BABEL_ENV === 'es' ? false : 'commonjs'
      }
    ]
  ]
}
