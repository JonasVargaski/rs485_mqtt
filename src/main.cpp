#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include "Ticker.h"
#include <PubSubClient.h>
#include <ModbusMaster.h>

#include "utils.h"
#include "config.h"
#include "ota.h"
#include "web_server.h"

ModbusMaster modbus;
SoftwareSerial softSerial(4, 5);

WiFiClient wifi;
PubSubClient mqtt(wifi);

int currentRegister = 0;

void mqttPublishHandle()
{
  if (config.modbus.status == MB_STATUS_PUBLISH)
  {
    StaticJsonDocument<JSON_SIZE> json;
    JsonObject root = json.to<JsonObject>();
    root["id"] = hexToStr(ESP.getEfuseMac());
    root["rssi"] = WiFi.RSSI();

    JsonArray holding = root.createNestedArray("holding");
    for (int i = 0; i < config.modbus.holdingRegs.size(); i++)
      holding.add<uint16_t>(config.modbus.holdingRegs[i].value);

    JsonArray coil = root.createNestedArray("coil");
    for (int i = 0; i < config.modbus.coilRegs.size(); i++)
      coil.add<bool>(config.modbus.coilRegs[i].value == 1 ? true : false);

    char parsedJson[JSON_SIZE];
    serializeJson(json, parsedJson);
    Serial.print(F("\n[mqtt] Send to topic: "));
    Serial.println(config.mqtt.topic);
    Serial.println(parsedJson);
    mqtt.publish(config.mqtt.topic.c_str(), parsedJson);
    config.modbus.status = MB_STATUS_IDLE;
  }
}

Ticker taskLed(&toggleBuiltInLed, 400, 0, MILLIS);
Ticker taskMqtt(&mqttPublishHandle, 2000, 0, MILLIS);

void setup()
{
  delay(300);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_BUILTIN, INPUT_PULLUP);
  Serial.begin(115200, SERIAL_8N1);
  Serial.println(F("Booting..."));
  delay(2000);

  if (SPIFFS.begin())
    config.load(SPIFFS);
  else
    Serial.println(F("SPIFFS error. Using default configs"));

  Serial.print(F("Connecting to ssid: "));
  Serial.println(config.wifi.ssid);

  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(config.deviceId.c_str());
  WiFi.begin(config.wifi.ssid, config.wifi.pass);

  setupOTA();

  if (config.wifi.enableWebServer)
    setupWebServer();

  softSerial.begin(config.modbus.baudrate, (Config)config.modbus.serialType);
  modbus.begin(config.modbus.slaveId, softSerial);

  mqtt.setServer(config.mqtt.server, config.mqtt.port);
  mqtt.setBufferSize(JSON_SIZE);
  mqtt.setCallback([](char *topic, byte *message, unsigned int length) {});

  taskMqtt.interval(config.mqtt.interval);
  taskLed.start();
  taskMqtt.start();
}

void loop()
{
  yield();
  mqtt.loop();
  ArduinoOTA.handle();
  taskLed.update();
  taskMqtt.update();

  if (config.wifi.enableWebServer)
    dnsServer.processNextRequest();

  if (WiFi.status() == WL_CONNECTED)
  {
    if (!mqtt.connected())
    {
      taskLed.interval(550);
      mqtt.connect(config.mqtt.clientId.c_str());

      if (taskMqtt.state() == RUNNING)
        taskMqtt.pause();
    }
    else
    {
      taskLed.interval(250);

      if (taskMqtt.state() == PAUSED)
        taskMqtt.resume();
    }
  }
  else
  {
    taskLed.interval(1350);
  }

  if (config.modbus.status == MB_STATUS_IDLE)
  {
    if (config.modbus.holdingRegs.size() > 0)
    {
      modbus.clearResponseBuffer();
      int result = modbus.readHoldingRegisters(0, config.modbus.holdingRegs.size());
      if (result == modbus.ku8MBSuccess)
      {
        config.modbus.status = MB_STATUS_PUBLISH;
        for (int i = 0; i < config.modbus.holdingRegs.size(); i++)
          config.modbus.holdingRegs[i].value = modbus.getResponseBuffer(i);
      }
    }

    if (config.modbus.coilRegs.size() > 0)
    {
      modbus.clearResponseBuffer();
      int result = modbus.readCoils(0, config.modbus.coilRegs.size());
      if (result == modbus.ku8MBSuccess)
      {
        config.modbus.status = MB_STATUS_PUBLISH;
        for (int i = 0; i < config.modbus.coilRegs.size(); i++)
          config.modbus.coilRegs[i].value = modbus.getResponseBuffer(i);
      }
    }
  }
}