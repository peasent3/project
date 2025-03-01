const express = require('express');
const axios = require('axios');
const app = express();

const http = require('http');
const fs = require('fs');

const port = 3000;
const hostname = '192.168.1.122';

// Replace with your ESP32-CAM IP address
const esp32CamUrl = 'http://192.168.1.104/capture';

app.get('/capture', async (req, res) => {
  try {
    const response = await axios.get(esp32CamUrl, { responseType: 'arraybuffer' });
    res.set('Content-Type', 'image/jpeg');
    res.send(response.data);
  } catch (error) {
    res.status(500).send('Error capturing image');
  }
});

const server = http.createServer((req,res) => {
    res.writeHead(200, {'Content-Type': 'text/html'});

      // read the html file
    fs.readFile('project.html', (err, data) => {
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

//app.listen(port, () => {
//  console.log(`Server running at http://localhost:${port}`);
//});