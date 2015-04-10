socket = require('socket.io-client') 'http://localhost:3000'
config = require './config.json'
fs = require 'fs'

try
    fs.unlinkSync './mouseEvents.txt'
catch err
    # ignore

socket.on 'connect', () =>

    console.log 'connected'


socket.on 'mouseEvent', (data) =>
    console.log data
    fs.appendFile './mouseEvents.txt', 'xdotool mousemove #data\n', (err) =>
        return console.log(err) if err


socket.on 'disconnect', () =>
    console.log 'client disconnect'
