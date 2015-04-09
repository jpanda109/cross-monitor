var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);


server.listen(3000, function() {
    console.log('Server listening at port %d', 3000);
});


io.on('connection', function(socket) {
    console.log('new connection from ' +  socket.handshake.address)

});
