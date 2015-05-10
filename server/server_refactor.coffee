express = require 'express'
app = express()
server = require('http').createServer(app)
io = require('socket.io')(server)
child_process = require 'child_process'

numScreens = 0
curScreen = 0
screenToSocket = {}

server.listen 3000, () =>
  console.log 'server listening at port %d', 3000

  mouse = child_process.spawn './releasesrc/MoveMouse', {
    stdio: ['pipe', null, null]
  }
  mouseInputStream = mouse.stdin
  eventListener = child_process.spawn './releasesrc/EventListener', {
    stdio: ['pipe', 'pipe', 'pipe']
  }

  eventInputStream = eventListener.stdin
  eventInputStream.setEncoding 'utf8'
  eventInputStream.write 'MovementOnly\n'

  eventOutputStream = eventListener.stdout
  eventOutputStream.setEncoding 'utf8'
  eventOutputStream.on 'redeable', () =>
    event = eventOutputStream.read().trim()
    console.log event
    info = mouseEvent.split(' ')
    if info[0] == 'MouseMove'
      curX = parseInt info[1]
      curY = parseInt info[2]

      if curX >= 1919 and curScreen < numScreens
        mouseInputStream.write 'MoveMouse 2 %d\n', curY
        curScreen += 1
        if curScreen != 0
          eventInputStream.write 'CatchAll\n'
      else if curX <= 1 and curScreen > 0
        mouseInputStream.write 'MoveMouse 1918 %d\n', curY
        curScreen -= 1
        if curScreen == 0
          eventInputStream.write 'MovementOnly\n'

    if curScreen != 0
      io.sockets.connected[screenToSocket[curScreen]].emit 'event', event


io.on 'connection', (socket) =>

  console.log 'new connection from ' + socket.handshake.address

  numScreens += 1
  screenToSocket[numScreens] = socket.id

  socket.on 'disconnect', () =>
    numScreens -= 1
