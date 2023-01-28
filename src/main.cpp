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

int battery;

void notify()
{
  //--- Digital cross/square/triangle/circle button events ---
  if (Ps3.event.button_down.cross)
    Logger.println("Started pressing the cross button");
  if (Ps3.event.button_up.cross)
    Logger.println("Released the cross button");

  if (Ps3.event.button_down.square)
    Logger.println("Started pressing the square button");
  if (Ps3.event.button_up.square)
    Logger.println("Released the square button");

  if (Ps3.event.button_down.triangle)
    Logger.println("Started pressing the triangle button");
  if (Ps3.event.button_up.triangle)
    Logger.println("Released the triangle button");

  if (Ps3.event.button_down.circle)
    Logger.println("Started pressing the circle button");
  if (Ps3.event.button_up.circle)
    Logger.println("Released the circle button");

  //--------------- Digital D-pad button events --------------
  if (Ps3.event.button_down.up)
    Logger.println("Started pressing the up button");
  if (Ps3.event.button_up.up)
    Logger.println("Released the up button");

  if (Ps3.event.button_down.right)
    Logger.println("Started pressing the right button");
  if (Ps3.event.button_up.right)
    Logger.println("Released the right button");

  if (Ps3.event.button_down.down)
    Logger.println("Started pressing the down button");
  if (Ps3.event.button_up.down)
    Logger.println("Released the down button");

  if (Ps3.event.button_down.left)
    Logger.println("Started pressing the left button");
  if (Ps3.event.button_up.left)
    Logger.println("Released the left button");

  //------------- Digital shoulder button events -------------
  if (Ps3.event.button_down.l1)
    Logger.println("Started pressing the left shoulder button");
  if (Ps3.event.button_up.l1)
    Logger.println("Released the left shoulder button");

  if (Ps3.event.button_down.r1)
    Logger.println("Started pressing the right shoulder button");
  if (Ps3.event.button_up.r1)
    Logger.println("Released the right shoulder button");

  //-------------- Digital trigger button events -------------
  if (Ps3.event.button_down.l2)
    Logger.println("Started pressing the left trigger button");
  if (Ps3.event.button_up.l2)
    Logger.println("Released the left trigger button");

  if (Ps3.event.button_down.r2)
    Logger.println("Started pressing the right trigger button");
  if (Ps3.event.button_up.r2)
    Logger.println("Released the right trigger button");

  //--------------- Digital stick button events --------------
  if (Ps3.event.button_down.l3)
    Logger.println("Started pressing the left stick button");
  if (Ps3.event.button_up.l3)
    Logger.println("Released the left stick button");

  if (Ps3.event.button_down.r3)
    Logger.println("Started pressing the right stick button");
  if (Ps3.event.button_up.r3)
    Logger.println("Released the right stick button");

  //---------- Digital select/start/ps button events ---------
  if (Ps3.event.button_down.select)
    Logger.println("Started pressing the select button");
  if (Ps3.event.button_up.select)
    Logger.println("Released the select button");

  if (Ps3.event.button_down.start)
    Logger.println("Started pressing the start button");
  if (Ps3.event.button_up.start)
    Logger.println("Released the start button");

  if (Ps3.event.button_down.ps)
    Logger.println("Started pressing the Playstation button");
  if (Ps3.event.button_up.ps)
    Logger.println("Released the Playstation button");

  //---------------- Analog stick value events ---------------
  if (abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2)
  {
    Logger.print("Moved the left stick:");
    Logger.print(" x=");
    Logger.print(Ps3.data.analog.stick.lx);
    Logger.print(" y=");
    Logger.print(Ps3.data.analog.stick.ly);
    Logger.println("");
  }

  if (abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2)
  {
    Logger.print("Moved the right stick:");
    Logger.print(" x=");
    Logger.print(Ps3.data.analog.stick.rx);
    Logger.print(" y=");
    Logger.print(Ps3.data.analog.stick.ry);
    Logger.println("");
  }

  //--------------- Analog D-pad button events ----------------
  if (abs(Ps3.event.analog_changed.button.up))
  {
    Logger.print("Pressing the up button: ");
    Logger.println(Ps3.data.analog.button.up);
  }

  if (abs(Ps3.event.analog_changed.button.right))
  {
    Logger.print("Pressing the right button: ");
    Logger.println(Ps3.data.analog.button.right);
  }

  if (abs(Ps3.event.analog_changed.button.down))
  {
    Logger.print("Pressing the down button: ");
    Logger.println(Ps3.data.analog.button.down);
  }

  if (abs(Ps3.event.analog_changed.button.left))
  {
    Logger.print("Pressing the left button: ");
    Logger.println(Ps3.data.analog.button.left);
  }

  //---------- Analog shoulder/trigger button events ----------
  if (abs(Ps3.event.analog_changed.button.l1))
  {
    Logger.print("Pressing the left shoulder button: ");
    Logger.println(Ps3.data.analog.button.l1);
  }

  if (abs(Ps3.event.analog_changed.button.r1))
  {
    Logger.print("Pressing the right shoulder button: ");
    Logger.println(Ps3.data.analog.button.r1);
  }

  if (abs(Ps3.event.analog_changed.button.l2))
  {
    Logger.print("Pressing the left trigger button: ");
    Logger.println(Ps3.data.analog.button.l2);
  }

  if (abs(Ps3.event.analog_changed.button.r2))
  {
    Logger.print("Pressing the right trigger button: ");
    Logger.println(Ps3.data.analog.button.r2);
  }

  //---- Analog cross/square/triangle/circle button events ----
  if (abs(Ps3.event.analog_changed.button.triangle))
  {
    Logger.print("Pressing the triangle button: ");
    Logger.println(Ps3.data.analog.button.triangle);
  }

  if (abs(Ps3.event.analog_changed.button.circle))
  {
    Logger.print("Pressing the circle button: ");
    Logger.println(Ps3.data.analog.button.circle);
  }

  if (abs(Ps3.event.analog_changed.button.cross))
  {
    Logger.print("Pressing the cross button: ");
    Logger.println(Ps3.data.analog.button.cross);
  }

  if (abs(Ps3.event.analog_changed.button.square))
  {
    Logger.print("Pressing the square button: ");
    Logger.println(Ps3.data.analog.button.square);
  }

  //---------------------- Battery events ---------------------
  // if (battery != Ps3.data.status.battery)
  // {
  //   battery = Ps3.data.status.battery;
  //   Logger.print("The controller battery is ");
  //   if (battery == ps3_status_battery_charging)
  //     Logger.println("charging");
  //   else if (battery == ps3_status_battery_full)
  //     Logger.println("FULL");
  //   else if (battery == ps3_status_battery_high)
  //     Logger.println("HIGH");
  //   else if (battery == ps3_status_battery_low)
  //     Logger.println("LOW");
  //   else if (battery == ps3_status_battery_dying)
  //     Logger.println("DYING");
  //   else if (battery == ps3_status_battery_shutdown)
  //     Logger.println("SHUTDOWN");
  //   else
  //     Logger.println("UNDEFINED");
  // }
}

void onConnect()
{
  Logger.println("Connected.");
}

void setup()
{
  Serial.begin(115200);

  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW);
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

  //  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin(PS3_CONTROLLER_MAC);

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
  // Logger.println("Here is my message");
  // delay(500);
  // digitalWrite(STATUS_LED_PIN, HIGH);
  // delay(500);
  // digitalWrite(STATUS_LED_PIN, LOW);
  Logger.println(Ps3.data.analog.stick.lx);
  delay(100);
}