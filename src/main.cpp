#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Ps3Controller.h>

#include "udp_logger.h"
#include "Hbridge.h"
#include "wifi_credentials.h"

#define PS3_CONTROLLER_MAC "2c:81:58:a7:b9:f7"
// ESP32 Dev
#define STATUS_LED_PIN 2

// ESP32 Cam
// #define STATUS_LED_PIN 33

#define UDP_LOG_PORT 5555

AsyncWebServer server(80);
UdpLoggerClass Logger;

// HBridgeChannel drivingWheels(26, 27, 14, 0);
//  HBridgeChannel steering();

enum ePS3ControllerState
{
  eDisconnected,
  eConnected,
  eBatteryLow
};
ePS3ControllerState controllerState = eDisconnected;
ePS3ControllerState prevControllerState = controllerState;

void setup()
{
  Serial.begin(115200);

  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW);

  Ps3.begin(PS3_CONTROLLER_MAC);

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
  if (Ps3.isConnected())
  {
    controllerState = eConnected;

    if (Ps3.data.status.battery <= ps3_status_battery_low)
    {
      controllerState = eBatteryLow;
    }
  }
  else
  {
    controllerState = eDisconnected;
  }

  if (prevControllerState != controllerState)
  {
    switch (controllerState)
    {
    case eConnected:
      Logger.println("Controller connected");
      break;
    case eDisconnected:
      Logger.println("Controller disconnected");
      break;
    case eBatteryLow:
      Logger.println("Controller battery low");
      break;
    }
    prevControllerState = controllerState;
  }

  if (controllerState == eDisconnected)
  {
    //    drivingWheels.velocity(0);
  }
  else
  {
    //    drivingWheels.velocity(Ps3.data.analog.stick.rx); // -128 ... 127, default -1
    Logger.println(Ps3.data.analog.stick.ry);
  }

  delay(1000);
}