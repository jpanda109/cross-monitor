var socket = require('socket.io-client')('http://localhost:3000');
var config = require('./config.json');
var fs = require('fs');

try {
    fs.unlinkSync('./mouseEvents.txt');
}
catch (err) {}


socket.on('connect', function() {

    console.log('connected');

    socket.emit('validateSession', config.PASSWORD);

});


socket.on('mouseEvent', function(data) {
    console.log(data);
    fs.appendFile('./mouseEvents.txt', 'xdotool mousemove ' + data + '\n', function(err) {
        if (err) {
            return console.log(err);
        }
    });
});


socket.on('randomEvent', function(data) {
    console.log(data);
});


socket.on('disconnect', function() {

    console.log('disconnected');

});
