#include <WiFi.h>
#include <WiFiClient.h>
#include "esp_camera.h"
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Camera configuration
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

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

  // Route to stream MJPEG video
  server.on("/video", HTTP_GET, [](AsyncWebServerRequest *request){
    WiFiClient client = request->client();
    camera_fb_t *fb = NULL;

    request->send_P(200, "multipart/x-mixed-replace; boundary=frame", [](uint8_t *buffer, size_t maxLen, size_t index){
      static size_t len = 0;
      camera_fb_t *fb = esp_camera_fb_get();

      if (!fb) {
        Serial.println("Camera capture failed");
        return 0;
      }

      len = fb->len;
      memcpy(buffer, fb->buf, len);
      esp_camera_fb_return(fb);

      return len;
    });
  });

  // Start server
  server.begin();
}

void loop() {
  // Nothing to do in the loop
}

