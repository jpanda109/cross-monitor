var socket = require('socket.io-client')('http://localhost:3000');
var config = require('./config.json');


socket.on('connect', function() {

    console.log('connected');

    socket.emit('validateSession', config.PASSWORD);

});


socket.on('mouseEvent', function(data) {
    console.log(data);
});


socket.on('randomEvent', function(data) {
    console.log(data);
});


socket.on('disconnect', function() {

    console.log('disconnected');

});
