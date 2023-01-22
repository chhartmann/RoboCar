#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
// #include <Ps3Controller.h>

#include "udp_logger.h"
#include "Hbridge.h"
#include "wifi_credentials.h"

// ESP32 Dev
#define STATUS_LED_PIN 2

// ESP32 Cam
// #define STATUS_LED_PIN 33

#define UDP_LOG_PORT 5555

AsyncWebServer server(80);
UdpLoggerClass Logger;

void setup()
{
  Serial.begin(115200);

  pinMode(STATUS_LED_PIN, OUTPUT);
  //  Ps3.begin("YourDeviceAddress");

  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname("RoboCar");
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Logger.init(UDP_LOG_PORT);
  Logger.println("\033cWiFi connected"); // clear screen

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
  Logger.println("Here is my message");
  delay(500);
  digitalWrite(STATUS_LED_PIN, HIGH);
  delay(500);
  digitalWrite(STATUS_LED_PIN, LOW);
}