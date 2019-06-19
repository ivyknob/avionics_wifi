merge = require('webpack-merge')
CompressionPlugin = require('compression-webpack-plugin')
WebpackCleanPlugin = require('webpack-clean')
HtmlWebpackPlugin = require('html-webpack-plugin')
common = require('./webpack.common.config.coffee')

module.exports = merge(common, {
  mode: 'production'
  plugins: [
    new CompressionPlugin
      test: /avionics\.js$/
    new WebpackCleanPlugin(['main/html/avionics.js'])
    new HtmlWebpackPlugin
      filename: 'index.html'
      template: 'src/index.ejs'
      injext: false
      scriptSrc: 'avionics.js.gz'
  ]
})
