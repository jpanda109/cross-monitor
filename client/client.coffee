socket = require('socket.io-client') 'http://localhost:3000'
decryption = require './decryption.coffee'
config = require './config'


socket.on 'connect', () =>

  console.log 'connected'


socket.on 'event', (data) =>

  data = decryption.decrypt(data, config.PASSWORD)
  console.log data


socket.on 'disconnect', () =>

  console.log 'client disconnect'
