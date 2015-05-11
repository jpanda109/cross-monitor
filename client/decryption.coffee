crypto = require 'crypto'
algorithm = 'aes-256-ctr'

decrypt = (text, password='random123pass') ->
  decipher = crypto.createDecipher(algorithm, password)
  decrypted = decipher.update(text, 'hex', 'utf8')
  decrypted += decipher.final('utf8')
  return decrypted

exports.decrypt = decrypt
