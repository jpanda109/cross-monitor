var express = require('express');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var child_process = require('child_process');
var Readable = require('stream').Readable;

var config = require('./config.json');


server.listen(3000, function() {
    console.log('Server listening at port %d', 3000);
});


io.on('connection', function(socket) {
    console.log('new connection from ' + socket.handshake.address);

    valid_connection = false;

    var mouseTest = child_process.spawn('./mouseTest', {
        stdio: [
            0,
            'pipe',
            'pipe'
        ]
    });
    var inputStream = mouseTest.stdout;
    inputStream.setEncoding('utf8');
    inputStream.on('readable', function() {
        socket.emit('mouseEvent', inputStream.read());
    });
    var errStream = mouseTest.stderr;
    errStream.setEncoding('utf8');
    errStream.on('readable', function() {
        console.log(errStream.read());
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
