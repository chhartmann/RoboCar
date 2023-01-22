#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h>
#include <TelnetStream.h>
#include <Ps3Controller.h>

#include "wifi_credentials.h"

AsyncWebServer server(80);

Stream* logger = &TelnetStream;
//Stream* logger2 = &Serial;

void setupOTA() {
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      logger->println("Start updating " + type);

      // server.end();
      // TelnetStream.end();
    })
    .onEnd([]() {
      logger->println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      logger->printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      logger->printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) logger->println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) logger->println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) logger->println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) logger->println("Receive Failed");
      else if (error == OTA_END_ERROR) logger->println("End Failed");
    });

  ArduinoOTA.begin();
}

void setup() {
  Serial.begin(115200);

//  Ps3.begin("YourDeviceAddress");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  TelnetStream.begin();

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  setupOTA();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  server.begin();
  logger->println("HTTP server started");
  
}

void loop() {
  ArduinoOTA.handle();
  delay(1000);
  logger->println("alive");
//  if (Ps3.isConnected()){
//    Log.println("Controller connected!");
//  }

}