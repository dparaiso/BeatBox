var PORT = 8088;


const http = require('http');
const path = require('path');
const mime = require('mime');
const fs   = require('fs');

const server = http.createServer((req, res) => {
	var filePath = "";
	if (req.url != '/') {
		filePath = './public' + req.url;
	} else {
		filePath = './public/index.html';
	}
	requestListener(res, filePath);
});

server.listen(PORT, () => {
	console.log("Server listening on port " + PORT);
});

const requestListener = (res, filePath) => {
	fs.readFile(filePath, (err, data) => {
		if (err) {
			throw err;
		}
		else {
			res.writeHead(200, {"content-type": mime.lookup(path.basename(filePath))});
			res.end(data);
		}
	  })
};

var udpServer = require('./lib/udpServer');
udpServer.listen(server)

