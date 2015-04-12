express = require 'express'
app = express()
server = require('http').createServer app
io = require('socket.io') server
child_process = require 'child_process'

numScreens = 0
curScreen = 0

screenToSocket = {}

server.listen 3000, () =>
    console.log 'server listening at port %d', 3000

    mouseEventProc = child_process.spawn './echomouseevents', {
      stdio: ['pipe', 'pipe', 'pipe']
    }

    eventInputStream = mouseEventProc.stdin
    eventInputStream.setEncoding('utf8')
    eventInputStream.write('movement only\n')
    eventOutputStream = mouseEventProc.stdout
    eventOutputStream.setEncoding 'utf8'
    eventOutputStream.on 'readable', () =>
      mouseEvent = eventOutputStream.read().trim()
      info = mouseEvent.split ' '
      if info[0] == 'Mouse'  # if it's a mouse move event
        curX = parseInt(info[2])
        curY = parseInt(info[3])

        # move mouse to other side of screen to simulate going to diff screen
        if curX >= 1919 and curScreen < numScreens
          child_process.exec 'xdotool mousemove 2 ' + curY
          curScreen += 1
          if curScreen == 0
            console.log('movement only')
            eventInputStream.write('movement only\n')
          else
            eventInputStream.write('resume\n')

        if curX <= 1 and curScreen > 0
          child_process.exec 'xdotool mousemove 1918 ' + curY
          curScreen -= 1
          if curScreen == 0
            console.log('movement only')
            eventInputStream.write('movement only\n')
          else
            eventInputStream.write('resume\n')

        if curScreen != 0  # if not on server screen, emit
          io.sockets.connected[screenToSocket[curScreen]].emit 'mouseEvent', curX + ' ' + curY


io.on 'connection', (socket) =>

    console.log 'new connection from ' + socket.handshake.address

    numScreens += 1
    screenToSocket[numScreens] = socket.id

    socket.on 'disconnect', () =>
        numScreens -= 1

