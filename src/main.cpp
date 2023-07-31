#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include "Ticker.h"
#include <PubSubClient.h>
#include <ModbusMaster.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"

#include "pin_config.h"
#include "utils.h"
#include "config.h"
#include "web_server.h"

void mqttPublishHandle(void);

ModbusMaster modbus;
HardwareSerial Serial485(2);

WiFiClient wifi;
PubSubClient mqtt(wifi);

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, WS2812_PIN, CHANNEL, TYPE_GRB);

Ticker taskMqtt(&mqttPublishHandle, 2000, 0, MILLIS);

void setup()
{
  pinMode(RS485_EN_PIN, OUTPUT);
  digitalWrite(RS485_EN_PIN, HIGH);

  pinMode(RS485_SE_PIN, OUTPUT);
  digitalWrite(RS485_SE_PIN, HIGH);

  pinMode(PIN_5V_EN, OUTPUT);
  digitalWrite(PIN_5V_EN, HIGH);

  pinMode(CAN_SE_PIN, OUTPUT);
  digitalWrite(CAN_SE_PIN, LOW);

  strip.begin();
  strip.setBrightness(10);
  strip.setLedColor(0, 0, 0, 0); // OFF

  delay(350);
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

  if (config.wifi.enableWebServer)
    setupWebServer();

  Serial485.begin(config.modbus.baudrate, SERIAL_8E1, RS485_RX_PIN, RS485_TX_PIN);
  modbus.begin(config.modbus.slaveId, Serial485);

  mqtt.setServer(config.mqtt.server, config.mqtt.port);
  mqtt.setBufferSize(JSON_SIZE);
  mqtt.setCallback([](char *topic, byte *message, unsigned int length) {});

  taskMqtt.interval(config.mqtt.interval);
  taskMqtt.start();
}

void loop()
{
  yield();
  mqtt.loop();
  taskMqtt.update();

  if (config.wifi.enableWebServer)
    dnsServer.processNextRequest();

  if (WiFi.status() == WL_CONNECTED)
  {
    if (!mqtt.connected())
    {
      strip.setLedColor(LED_COLOR_SERVER_ERROR);
      mqtt.connect(config.mqtt.clientId.c_str());

      if (taskMqtt.state() == RUNNING)
        taskMqtt.pause();
    }
    else if (taskMqtt.state() == PAUSED)
      taskMqtt.resume();
  }
  else
  {
    strip.setLedColor(LED_COLOR_WIFI_ERROR);
    if (taskMqtt.state() == RUNNING)
      taskMqtt.pause();
  }

  if (WiFi.status() == WL_CONNECTED && config.modbus.status == MB_STATUS_IDLE)
  {
    uint8_t result;
    int arraySize;
    int partAmount;
    int startIndex;
    int endIndex;

    if (config.modbus.holdingRegs.size() > 0)
    {
      arraySize = config.modbus.holdingRegs.size();
      partAmount = (arraySize + config.modbus.recordsPerRead - 1) / config.modbus.recordsPerRead;

      for (int i = 0; i < partAmount; i++)
      {
        startIndex = i * config.modbus.recordsPerRead;
        endIndex = min((i + 1) * config.modbus.recordsPerRead - 1, arraySize - 1);

        modbus.clearResponseBuffer();
        result = modbus.readHoldingRegisters(startIndex, endIndex);
        if (result == modbus.ku8MBSuccess)
        {
          config.modbus.status = MB_STATUS_PUBLISH;
          for (int j = startIndex; j <= endIndex; j++)
            config.modbus.holdingRegs[j] = modbus.getResponseBuffer(j - startIndex);
        }
        else
          strip.setLedColor(LED_COLOR_MODBUS_ERROR);
      }
    }

    if (config.modbus.coilRegs.size() > 0)
    {
      arraySize = config.modbus.coilRegs.size();
      partAmount = (arraySize + config.modbus.recordsPerRead - 1) / config.modbus.recordsPerRead;

      for (int i = 0; i < partAmount; i++)
      {
        startIndex = i * config.modbus.recordsPerRead;
        endIndex = min((i + 1) * config.modbus.recordsPerRead - 1, arraySize - 1);

        modbus.clearResponseBuffer();
        result = modbus.readCoils(startIndex, endIndex);
        if (result == modbus.ku8MBSuccess)
        {
          config.modbus.status = MB_STATUS_PUBLISH;
          for (int j = startIndex; j <= endIndex; j++)
          {
            int bufferIndex = j - startIndex;
            config.modbus.coilRegs[j] = (modbus.getResponseBuffer(bufferIndex / 16) >> (bufferIndex % 16)) & 0x01;
          }
        }
        else
          strip.setLedColor(LED_COLOR_MODBUS_ERROR);
      }
    }
  }
}

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
      holding.add<uint16_t>(config.modbus.holdingRegs[i]);

    JsonArray coil = root.createNestedArray("coil");
    for (int i = 0; i < config.modbus.coilRegs.size(); i++)
      coil.add<byte>(config.modbus.coilRegs[i]);

    char parsedJson[JSON_SIZE];
    serializeJson(json, parsedJson);
    Serial.print(F("\n[mqtt] Send to topic: "));
    Serial.println(config.mqtt.topic);
    Serial.println(parsedJson);

    bool publish_ok = mqtt.publish(config.mqtt.topic.c_str(), parsedJson);

    if (publish_ok)
    {
      config.modbus.status = MB_STATUS_IDLE;
      strip.setLedColor(LED_COLOR_OK);
    }
    else
    {
      strip.setLedColor(LED_COLOR_SERVER_ERROR);
    }
  }
}
