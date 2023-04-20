#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <TaskScheduler.h>

#include "mqtt.h"
#include "utils.h"
#include "config.h"
#include "modbus.h"
#include "ota.h"
#include "web_server.h"
#include "setup_wifi.h"

Scheduler taskRunner;

#define BUILTIN_BUTTON = 15;

void syncModbusMqtt(void);

Task taskLED(500, TASK_FOREVER, &toggleBuiltInLed, &taskRunner, false);
Task taskSyncModbusMqtt(1000, TASK_FOREVER, &syncModbusMqtt, &taskRunner, false);

void syncModbusMqtt()
{
  if (!modbus.slave())
  {
    uint16_t res1 = 0;
    uint16_t res2 = 0;
    uint16_t res3 = 0;
    uint16_t res4 = 0;

    modbus.writeHreg(config.modbus.slaveId, 2, WiFi.status());
    modbusWaitTillDone();
    modbus.writeHreg(config.modbus.slaveId, 3, getRSSIInPercent());
    modbusWaitTillDone();

    modbus.readHreg(config.modbus.slaveId, 0, &res1, 1, modbusCallback);
    modbusWaitTillDone();
    modbus.readHreg(config.modbus.slaveId, 1, &res2, 1, modbusCallback);
    modbusWaitTillDone();
    modbus.readHreg(config.modbus.slaveId, 2, &res3, 1, modbusCallback);
    modbusWaitTillDone();
    modbus.readHreg(config.modbus.slaveId, 3, &res4, 1, modbusCallback);
    modbusWaitTillDone();

    char buffer[70];
    sprintf(buffer, "{\"40001\":%d, \"40002\":%d, \"40003\":%d, \"40004\":%d}", res1, res2, res3, res4);
    Serial.println(buffer);

    if (WiFi.status() == WL_CONNECTED)
      mqtt.publish(config.mqtt.modbusTopic, buffer);
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200, SERIAL_8N1);
  Serial.println(F("Booting"));

  if (SPIFFS.begin())
    config.load(SPIFFS);
  else
    Serial.println(F("SPIFFS error. Using default configs"));

  setupWifi();
  setupOTA();
  setupWebServer();
  setupModbusMaster();
  setupMqtt();
  taskLED.enableIfNot();
  taskSyncModbusMqtt.enableDelayed(2000);
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
}