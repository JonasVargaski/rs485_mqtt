#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <ModbusRTU.h>
#include "Ticker.h"
#include <PubSubClient.h>

#include "utils.h"
#include "config.h"
#include "ota.h"
#include "web_server.h"
#include "setup_wifi.h"

ModbusRTU modbus;
SoftwareSerial softSerial(4, 5);

WiFiClient wifi;
PubSubClient mqtt(wifi);

#define BUILTIN_BUTTON = 15;

int currentRegister = 0;

void mqttPublishHandle()
{
  if (currentRegister >= config.modbus.reg_count)
  {
    StaticJsonDocument<JSON_SIZE> json;
    JsonObject root = json.to<JsonObject>();
    root["id"] = hexToStr(ESP.getEfuseMac());

    JsonArray result = root.createNestedArray("result");
    for (int i = 0; i < config.modbus.reg_count; i++)
    {
      JsonObject object = result.createNestedObject();
      object["sId"] = config.modbus.registers[i].slave_id;
      object["addr"] = config.modbus.registers[i].addr;
      object["type"] = String(config.modbus.registers[i].type);
      object["value"] = config.modbus.registers[i].type == 'H' ? config.modbus.registers[i].h_value : config.modbus.registers[i].c_value;
    }

    char parsedJson[JSON_SIZE];
    serializeJson(json, parsedJson);
    Serial.print(F("\n[mqtt] Send to topic: "));
    Serial.println(config.mqtt.resultTopic);
    Serial.println(parsedJson);
    mqtt.publish(config.mqtt.resultTopic, parsedJson);
    currentRegister = 0;
  }
}

Ticker taskLed(&toggleBuiltInLed, 400, 0, MILLIS);
Ticker taskMqtt(&mqttPublishHandle, 2000, 0, MILLIS);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200, SERIAL_8N1);
  delay(800);
  Serial.println(F("Booting..."));

  if (SPIFFS.begin())
    config.load(SPIFFS);
  else
    Serial.println(F("SPIFFS error. Using default configs"));

  setupWifi();
  setupOTA();
  setupWebServer();

  softSerial.begin(config.modbus.baudrate, (Config)config.modbus.serialType);
  modbus.begin(&softSerial);
  modbus.master();

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
  modbus.task();
  mqtt.loop();
  ArduinoOTA.handle();
  dnsServer.processNextRequest();
  taskLed.update();
  taskMqtt.update();

  if (WiFi.status() == WL_CONNECTED)
  {
    if (!mqtt.connected())
    {
      taskLed.interval(550);
      mqtt.connect(config.mqtt.clientId);

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

  if (!modbus.slave() && config.modbus.reg_count > 0 && currentRegister <= config.modbus.reg_count)
  {
    if (config.modbus.registers[currentRegister].type == 'H')
      modbus.readHreg(config.modbus.registers[currentRegister].slave_id, config.modbus.registers[currentRegister].addr, &config.modbus.registers[currentRegister].h_value, 1);

    else if (config.modbus.registers[currentRegister].type == 'C')
      modbus.readCoil(config.modbus.registers[currentRegister].slave_id, config.modbus.registers[currentRegister].addr, &config.modbus.registers[currentRegister].c_value, 1);

    currentRegister += 1;
  }
}