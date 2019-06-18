path = require('path')

module.exports =
  mode: 'development'
  entry:
    avionics: './src/index.js'
  output:
    path: path.resolve(__dirname, 'main', 'html')
  module:
    rules: [{
      test: /\.css$/
      use: ['style-loader', 'css-loader']
    },{
      test: /\.html$/
      use: [{
        loader: 'html-loader',
        options: {
          minimize: true
        }
      }]
    }]
