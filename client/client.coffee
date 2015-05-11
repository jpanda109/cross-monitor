socket = require('socket.io-client') 'http://localhost:3000'


socket.on 'connect', () =>

  console.log 'connected'


socket.on 'event', (data) =>

  console.log data


socket.on 'disconnect', () =>

  console.log 'client disconnect'
