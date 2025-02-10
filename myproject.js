const http = require('http');
const fs = require('fs');

const hostname = 'bluenix.local';
const port = 8080;

const server = http.createServer((req,res) => {
    res.writeHead(200, {'Content-Type': 'text/html'});

      // read the html file
    fs.readFile('index.html', (err, data) => {
    if (err) {
      // handle error
      res.writeHead(404);
      res.write('File not found!');
    } else {
      // send the html code as the response
      res.write(data);
    }

    // end the response
    res.end();
  });
});

server.listen(port, hostname, () => {
    console.log('Server running at http://${hostname}:${port}/');
});