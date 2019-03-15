path = require('path')
CompressionPlugin = require('compression-webpack-plugin')
# WebpackCleanPlugin = require('webpack-clean')

module.exports =
  mode: 'production'
  entry:
    avionics: './src/index.js'
  output:
    path: path.resolve(__dirname, 'main', 'html')
  module:
    rules: [{
      test: /\.css$/
      use: ['style-loader', 'css-loader']
    }]
  plugins: [
    new CompressionPlugin
      test: /avionics\.js$/
    # new WebpackCleanPlugin(['main/html/avionics.js'])
  ]
