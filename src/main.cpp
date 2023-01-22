#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
// #include <Ps3Controller.h>

#include "udp_logger.h"
#include "wifi_credentials.h"

AsyncWebServer server(80);
UdpLoggerClass Logger;

void setup()
{
  Serial.begin(115200);

  //  Ps3.begin("YourDeviceAddress");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Logger.init(5555);

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Hi! I am ESP32."); });

  AsyncElegantOTA.begin(&server);
  server.begin();

  Logger.println("HTTP server started");
}

void loop()
{
  delay(1000);
  Logger.println("Here is my message");
  //  if (Ps3.isConnected()){
  //    Log.println("Controller connected!");
  //  }
}