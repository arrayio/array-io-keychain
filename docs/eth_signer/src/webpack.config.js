module.exports = {
  mode: 'development',
  entry: './ethSigner.js',
  output: {
    filename: 'main.js',
    path: __dirname + '/../dist',
  },
  resolve: {
    extensions: ['.js', '.json']
  },
};
