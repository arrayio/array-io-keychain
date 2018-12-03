var path = require('path');
var webpack = require('webpack');
var CircularDependencyPlugin = require('circular-dependency-plugin');

module.exports = {
  mode: 'development',
  entry: './index.tsx',
  output: {
    path: __dirname + '/build',
    filename: 'dapp.js',
  },
 
  module: {
    rules: [
      { test: /\.(ts|tsx)$/, loader: 'ts-loader' },
      {
        test: /\.css$/,
        loader:'style-loader!css-loader',
        exclude: /node_modules/
      }
    ]
  },
  plugins: [
    new CircularDependencyPlugin({
      // exclude detection of files based on a RegExp
      exclude: /a\.js|node_modules/,
      // add errors to webpack instead of warnings
      failOnError: true,
      // set the current working directory for displaying module paths
      cwd: process.cwd(),
    })
  ],
  stats: {
    colors: true
  },
  resolve: {
    // Add '.ts' and '.tsx' as resolvable extensions.
    extensions: ['.ts', '.tsx', '.js', '.json']
  },
  devtool: 'source-map',
};
