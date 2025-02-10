#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "esp_camera.h"

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Camera configuration
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// Create an instance of WebServer on port 80
WebServer server(80);

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Camera initialization
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL;
  config.ledc_timer = LEDC_TIMER;
  config.pin_d0 = 12;
  config.pin_d1 = 13;
  config.pin_d2 = 14;
  config.pin_d3 = 15;
  config.pin_d4 = 2;
  config.pin_d5 = 4;
  config.pin_d6 = 5;
  config.pin_d7 = 18;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 21;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Initialize camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  // Serve MJPEG stream at /video
  server.on("/video", HTTP_GET, [](){
    WiFiClient client = server.client();
    camera_fb_t *fb = NULL;

    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Connection", "close");
    server.send(200, "multipart/x-mixed-replace; boundary=frame", "");

    while (client.connected()) {
      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        return;
      }

      // Send MJPEG frame to client
      client.write("--frame\r\n");
      client.write("Content-Type: image/jpeg\r\n");
      client.write("Content-Length: ");
      client.write(String(fb->len).c_str());
      client.write("\r\n\r\n");
      client.write(fb->buf, fb->len);
      client.write("\r\n");

      esp_camera_fb_return(fb);

      delay(30);  // Stream frame every 30ms (approximately 30fps)
    }
  });

  // Start the web server
  server.begin();
}

void loop() {
  server.handleClient();  // Handle incoming requests
}
