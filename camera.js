const express = require('express');
const axios = require('axios');
const app = express();
const port = 3000;

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

app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});