#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <TaskScheduler.h>
#include <ModbusRTU.h>

#include "mqtt.h"
#include "utils.h"
#include "config.h"
// #include "modbus.h"
#include "ota.h"
#include "web_server.h"
#include "setup_wifi.h"
#include <SoftwareSerial.h>

Scheduler taskRunner;
ModbusRTU modbus;
SoftwareSerial softSerial(4, 5);

#define BUILTIN_BUTTON = 15;

int currentRegister = 0;

void mqttPublishHandle()
{
  if (currentRegister >= config.modbus.reg_count)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      StaticJsonDocument<JSON_SIZE> json;
      JsonObject root = json.to<JsonObject>();
      root["deviceId"] = hexToStr(ESP.getEfuseMac());

      JsonArray result = root.createNestedArray("result");
      for (int i = 0; i < config.modbus.reg_count; i++)
      {
        JsonObject object = result.createNestedObject();
        object["slave_id"] = config.modbus.registers[i].slave_id;
        object["addr"] = config.modbus.registers[i].addr;
        object["type"] = String(config.modbus.registers[i].type);
        object["value"] = config.modbus.registers[i].type == 'H' ? config.modbus.registers[i].h_value : config.modbus.registers[i].c_value;
      }

      char parsedJson[JSON_SIZE];
      serializeJson(json, parsedJson);
      Serial.println(parsedJson);
      Serial.println("");
      mqtt.publish(config.mqtt.resultTopic, parsedJson);
    }

    currentRegister = 0;
  }
}

Task taskLED(500, TASK_FOREVER, &toggleBuiltInLed, &taskRunner, false);
Task mqttPusblishTask(1000, TASK_FOREVER, &mqttPublishHandle, &taskRunner, false);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200, SERIAL_8N1);
  Serial.println(F("Booting"));

  if (SPIFFS.begin())
    config.load(SPIFFS);
  else
    Serial.println(F("SPIFFS error. Using default configs"));

  softSerial.begin(config.modbus.baudrate, (Config)config.modbus.serialType);
  modbus.begin(&softSerial);
  modbus.master();

  setupWifi();
  setupOTA();
  setupWebServer();
  // setupModbusMaster();
  setupMqtt();
  mqttPusblishTask.setInterval(config.mqtt.interval);
  mqttPusblishTask.enableIfNot();
  taskLED.enableIfNot();
}

void loop()
{
  yield();
  reconnectMqtt();

  modbus.task();
  mqtt.loop();
  ArduinoOTA.handle();
  taskRunner.execute();
  dnsServer.processNextRequest();

  if (!modbus.slave() && config.modbus.reg_count > 0 && currentRegister <= config.modbus.reg_count)
  {
    if (config.modbus.registers[currentRegister].type == 'H')
      modbus.readHreg(config.modbus.registers[currentRegister].slave_id, config.modbus.registers[currentRegister].addr, &config.modbus.registers[currentRegister].h_value, 1);

    else if (config.modbus.registers[currentRegister].type == 'C')
      modbus.readCoil(config.modbus.registers[currentRegister].slave_id, config.modbus.registers[currentRegister].addr, &config.modbus.registers[currentRegister].c_value, 1);

    currentRegister += 1;
  }
}