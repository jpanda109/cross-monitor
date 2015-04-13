socket = require('socket.io-client') 'http://localhost:3000'
encryption = require '../common/encyption.coffee'
algorithm = 'aes-256-ctr'
config = require './config.json'
password = config.PASSWORD


socket.on 'connect', () =>

  console.log 'connected'


socket.on 'event', (data) =>

  decrypted_data = encryption.decrypt data, algorithm, password
  console.log decrypted_data


socket.on 'disconnect', () =>

  console.log 'client disconnect'
