"use strict";

const socketio = require('socket.io');
const dgram = require('dgram');

exports.listen = (server) => {
	const io = socketio.listen(server);
	io.set('log level', 1);
	io.sockets.on('connection', (socket) => {
		
		parseMessages(socket);
	});
};

function parseMessages(socket) {
	socket.on('beatBoxCommand', (data) => {
		const HOST = '192.168.7.2';
		const PORT = 12345;
		var client = dgram.createSocket('udp4');
		client.send(data, 0, data.length, PORT, HOST, (err, bytes) => {
			if (err) 
				throw err;
			console.log('UDP message sent to ' + HOST +':'+ PORT);
		});

		client.on('listening', function () {
			var address = client.address();
			console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		});
		// Handle an incoming message over the UDP from the local application.
		client.on('message', (msg, rem) => {
			console.log("UDP Client: message " + rem.address + ':' + rem.port +' - ' + msg);
			var reply = msg.toString('utf8')
			socket.emit('beatBoxCommandReply', reply);
			client.close();
		});
	});
};