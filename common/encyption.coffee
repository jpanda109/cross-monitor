encrypt = (text, algorithm, password) =>
  cipher = crypto.createCipher algorithm, password
  crypted = cipher.update text, 'utf8', 'hex'
  crypted += cipher.final 'hex'
  return crypted

decrypt = (text, algorithm, password) =>
  decipher = crypto.createDecipher algorithm, password
  dec = decipher.update text, 'hex', 'utf8'
  dec += decipher.final 'utf8'
  return dec

exports.encrypt = encrypt
exports.decrypt = decrypt
