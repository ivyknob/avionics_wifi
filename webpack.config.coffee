path = require('path')

module.exports =
  mode: 'production'
  entry:
    avionics: './src/avionics.js'
  output:
    path: path.resolve(__dirname, 'main', 'html')
