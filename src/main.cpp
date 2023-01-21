#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <TLog.h>
#include <TelnetSerialStream.h>
#include <Ps3Controller.h>

#include "wifi_credentials.h"

AsyncWebServer server(80);

TelnetSerialStream telnetSerialStream = TelnetSerialStream();
//TODO: add webserialstream and syslogstream?

void setup() {
  Serial.begin(115200);

  Ps3.begin("YourDeviceAddress");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Log.addPrintStream(std::make_shared<TelnetSerialStream>(telnetSerialStream));

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
  
}

void loop() {
  Log.loop();

  // Say something every seconds.
  static unsigned  long last_report = millis();
  if (millis() - last_report < 1 * 1000)
    return;

  static int i = 0;
  Log.printf("Hello number %d from the loop\n", i++);

  if (Ps3.isConnected()){
    Log.println("Controller connected!");
  }

  last_report = millis();
}