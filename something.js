const express = require('express');
const WebSocket = require('ws');
const http = require('http');

const app = express();
const server = http.createServer(app);

// WebSocket server setup
const wss = new WebSocket.Server({ server });

// Serve HTML page with video stream
app.get('/', (req, res) => {
  res.sendFile('/index.html');
});

// Handle WebSocket connections
wss.on('connection', (ws) => {
  console.log('Client connected');
  
  // Fetch the MJPEG stream from the ESP32
  const esp32Stream = require('http').get('http://192.168.1.104/capture', (res) => {
    res.on('data', (chunk) => {
      // Send the MJPEG frame to the client
      ws.send(chunk);
    });
  });

  ws.on('close', () => {
    console.log('Client disconnected');
    esp32Stream.destroy(); // Close the ESP32 stream when WebSocket is closed
  });
});

// Start the server
server.listen(3000, () => {
  console.log('Server started on port 3000');
});
