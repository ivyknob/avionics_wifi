path = require('path')
CompressionPlugin = require('compression-webpack-plugin')

module.exports =
  mode: 'production'
  entry:
    avionics: './src/avionics.js'
  output:
    path: path.resolve(__dirname, 'main', 'html')
  plugins: [
    new CompressionPlugin
      test: /avionics\.js$/
  ]
