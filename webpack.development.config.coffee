merge = require('webpack-merge')
path = require('path')
common = require('./webpack.common.config.coffee')
HtmlWebpackPlugin = require('html-webpack-plugin')

module.exports = merge(common, {
  mode: 'development'
  devServer:
    contentBase: path.join(__dirname, 'main', 'html')
  watchOptions: { ignored: /node_modules/ }
  plugins: [
    new HtmlWebpackPlugin
      filename: 'index.html'
      template: 'src/index.ejs'
      inject: false
      scriptSrc: 'avionics.js'
  ]
})
