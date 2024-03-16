"use strict";
/*
 * Respond to commands over a websocket to relay UDP commands to a local program
 */

// var socketio = require('socket.io');
var socketio = require('socket.io');
var io;

// var dgram = require('dgram');
var dgram = require('dgram');

exports.listen = (server) => {
	io = socketio.listen(server);
	io.set('log level', 1);
	
	io.sockets.on('connection', function(socket) {
		parseMessages(socket);
	});
};

function parseMessages(socket) {
	socket.on('beatBoxCommand', function(data) {
		const HOST = '192.168.7.2';
		const PORT = 12345;
	
		var client = dgram.createSocket('udp4');

		// client.bind(PORT, HOST, () => {
		// 	if(err) {
		// 		throw err;
		// 	}
		// });
		// console.log("client bind successful");
		// client.connect(PORT, HOST, () => {
		// 	if(err) {
		// 		throw err;
		// 	}
		// });
		// console.log("client connect successful");
		client.send(data, 0, data.length, PORT, HOST, (err, bytes) => {
			if (err) 
				throw err;
			console.log('UDP message sent to ' + HOST +':'+ PORT);
		});

		// client.on('listening', function () {
		// 	var address = client.address();
		// 	console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		// });
		// Handle an incoming message over the UDP from the local application.
		client.on('message', (msg, rem) => {
			console.log("UDP Client: message " + rem.address + ':' + rem.port +' - ' + msg);

			var reply = msg.toString('utf8')
			socket.emit('commandReply', reply);

			if(reply.includes("Program terminating")) {
				console.log("closing"); 
				client.close();
			}
			// client.close();
		});
		// client.on("UDP Client: close", function() {
		// 	console.log("closed");
		// });
		// client.on("UDP Client: error", function(err) {
		// 	console.log("error: ",err);
		// });
	});
};