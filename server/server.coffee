express = require 'express'
app = express()
server = require('http').createServer(app)
io = require('socket.io')(server)
child_process = require 'child_process'

numScreens = 0
curScreen = 0
sockets = []

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
  eventOutputStream.on 'readable', () =>
    event = eventOutputStream.read().trim()
    info = event.split(' ')
    if info[0] == 'Mouse'
      if info[1] == 'Move'
        curX = parseInt info[2]
        curY = parseInt info[3]

        if curX >= 1919 and curScreen < numScreens
          console.log 'hi'
          mouseInputStream.write 'MoveMouse 2 ' + curY + '\n'
          curScreen += 1
          if curScreen != 0
            eventInputStream.write 'CatchAll\n'
        else if curX <= 1 and curScreen > 0
          mouseInputStream.write 'MoveMouse 1918 ' + curY + '\n'
          curScreen -= 1
          if curScreen == 0
            eventInputStream.write 'MovementOnly\n'

    if curScreen != 0
      io.sockets.connected[sockets[curScreen-1]].emit 'event', event


io.on 'connection', (socket) =>

  console.log 'new connection from ' + socket.handshake.address
  console.log 'Accept connection? [Y/N]'
  process.stdin.resume()
  process.stdin.setEncoding('utf8')
  process.stdin.on 'data', (text) =>
    text = text.trim()
    if text == 'Y'
      numScreens += 1
      sockets.push(socket.id)
    else
      socket.disconnect()

  socket.on 'disconnect', () =>
    numScreens -= 1
    index = sockets.indexOf(socket.id)
    sockets.splice(index, 1)
    console.log socket.handshake.address + ' disconnect'
