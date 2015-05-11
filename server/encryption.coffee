crypto = require 'crypto'
algorithm = 'aes-256-ctr'

encrypt = (text, password = 'random123pass') ->
  cipher = crypto.createCipher(algorithm, password)
  crypted = cipher.update(text, 'utf8', 'hex')
  crypted += cipher.final('hex')
  return crypted

exports.encrypt = encrypt
