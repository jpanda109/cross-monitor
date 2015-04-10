SocketContainer = ->
  this._sockets = [0]
  this._mainScreen = 0


SocketContainer.prototype.addSocket = (socketId, i) ->
  if i > this._mainScreen.length
    throw new Error
  if i <= this._mainScreen
    this._mainScreen += 1
  this._sockets.splice(i, 0, socketId)


SocketContainer.prototype.removeSocket = (socketId) ->
  this._sockets = this._sockets.filter socket, i =>
    if socket != socketId
      if i < this._mainScreen
        this._mainScreen -= 1
      return false
    return true

SocketContainer.prototype.getSockets = ->
  return this._sockets

SocketContainer.prototype.getMainScreen = ->
  return this._mainScreen

container = new SocketContainer()
container.addSocket 10, 1
console.log container.getSockets().join()
console.log container.getMainScreen()
container.addSocket 20, 2
console.log container.getSockets().join()
console.log container.getMainScreen()
container.addSocket 30, 0
console.log container.getSockets().join()
console.log container.getMainScreen()
container.addSocket 40, 3
console.log container.getSockets().join()
console.log container.getMainScreen()


module.exports = SocketContainer
