
const path = require("path")
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
    entry: "./main.js",
    mode: "development",
    output: {
        path: path.resolve(__dirname, "dist"),
        filename: "bundle.js"
    },
    plugins: [
        new HtmlWebpackPlugin({template: "./index.html"}),
    ],
    devServer:{ 
        static: {
            publicPath: path.join(__dirname, 'dist')
        },
        open: true,
        compress: true,
        port: 3000
    }
}
