var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);

var config = require('./config.json');


server.listen(3000, function() {
    console.log('Server listening at port %d', 3000);
});


io.on('connection', function(socket) {
    console.log('new connection from ' + socket.handshake.address);

    valid_connection = false;

    app.get('/', function(req, res) {
        socket.emit('randomEvent', 'random');
        res.end('success');
    });

    socket.on('validateSession', function(data) {
        console.log('validating');
        console.log(data);
        console.log(config.PASSWORD);
        if (config.PASSWORD === data) {
            console.log('validated');
            valid_connection = true;
        }
        else {
            console.log('rejected');
        }
    });

});
