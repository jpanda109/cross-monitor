express = require 'express'
app = express()
server = require('http').createServer app
io = require('socket.io') server
child_process = require 'child_process'

config = require './config.json'

minScreen = 0
maxScreen = 0
curScreen = 0

screenToSocket = {}

server.listen 3000, () =>
    console.log 'server listening at port %d', 3000

    mouseTest = child_process.spawn './echomousepos', {
        stdio: [0, 'pipe', 'pipe']
    }

    inputStream = mouseTest.stdout
    inputStream.setEncoding 'utf8'
    lastX = '0'
    lastY = '0'
    inputStream.on 'readable', () =>
        mouseEvent = inputStream.read()
        info = mouseEvent.split ' '
        curX = info[0].substr 2
        curY = info[1].substr 2
        if parseInt(curX) >= 1919 and curX != lastX and curScreen < maxScreen
            child_process.exec 'xdotool mousemove 2 ' + curY, (err, stdout, stderr) =>
                if err
                    console.log 'exec error: ' + err
            curScreen += 1
            console.log curScreen
        if parseInt(curX) <= 1 and curX != lastX and curScreen > minScreen
            child_process.exec 'xdotool mousemove 1918 ' + curY, (err, stdout, stderr) =>
                if err
                    console.log 'exec error: ' + err
                else
                    curScreen -= 1
                    console.log curScreen
        if curX != lastX or curY != lastY
            if curScreen != 0
                io.sockets.connected[screenToSocket[curScreen]].emit 'mouseEvent', curX + ' ' + curY
        lastX = curX
        lastY = curY

    errStream = mouseTest.stderr
    errStream.setEncoding 'utf8'
    errStream.on 'readable', () =>
        console.log errStream.read()


io.on 'connection', (socket) =>

    console.log 'new connection from ' + socket.handshake.address

    maxScreen += 1
    screenToSocket[maxScreen] = socket.id

    socket.on 'disconnect', () =>
        maxScreen -= 1

