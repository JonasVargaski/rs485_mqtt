#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_
#include "Arduino.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <vector>
#include "utils.h"

const size_t JSON_SIZE = JSON_OBJECT_SIZE(50) + 2048;

struct Register
{
  unsigned int address;
  u_int16_t value;
  Register(int _address, int _value = -1) : address(_address), value(_value) {}
};

enum ModbusStatus
{
  MB_STATUS_IDLE,
  MB_STATUS_PUBLISH,
};

struct AppConfig
{
  String deviceId = "Mqtt485Gateway-" + hexToStr(ESP.getEfuseMac());

  struct WifiConfig
  {
    char ssid[30] = "";
    char pass[30] = "";
    IPAddress apIP = IPAddress(192, 168, 4, 1);
    IPAddress netMsk = IPAddress(255, 255, 255, 0);
  } wifi;

  struct MqttConfig
  {
    String topic = "c5d81ff2/device/" + hexToStr(ESP.getEfuseMac());
    String clientId = randomUUID();
    char server[60] = "broker.mqtt-dashboard.com";
    unsigned int port = 1883;
    unsigned int interval = 1000;
  } mqtt;

  struct ModbusConfig
  {
    int baudrate = 9600;
    int serialType = 3;
    int slaveId = 1;
    ModbusStatus status = MB_STATUS_IDLE;
    std::vector<Register> holdingRegs;
    std::vector<Register> coilRegs;
  } modbus;

  void load(FS &fs)
  {
    if (fs.exists((F("/config.json"))))
    {
      File file = fs.open(F("/config.json"), "r");
      StaticJsonDocument<JSON_SIZE> json;
      DeserializationError err = deserializeJson(json, file);
      file.close();

      if (!err)
      {
        // wifi
        strlcpy(wifi.ssid, json[F("wifi")][F("ssid")] | wifi.ssid, sizeof(wifi.ssid));
        strlcpy(wifi.pass, json[F("wifi")][F("pass")] | wifi.pass, sizeof(wifi.pass));

        // mqtt
        strlcpy(mqtt.server, json[F("mqtt")][F("server")] | mqtt.server, sizeof(mqtt.server));
        mqtt.port = json[F("mqtt")][F("port")].as<int>() | mqtt.port;
        mqtt.interval = json[F("mqtt")][F("interval")].as<int>() | mqtt.interval;

        // modbus
        modbus.baudrate = json[F("modbus")][F("baudrate")].as<int>() | modbus.baudrate;
        modbus.serialType = json[F("modbus")][F("serialType")].as<int>() | modbus.serialType;

        // registers
        for (int i = 0; i < 20; i++)
        {
          modbus.holdingRegs.push_back(Register(i));
          modbus.coilRegs.push_back(Register(i));
        }

        return;
      }
      else
      {
        Serial.print(F("Error: deserializeJson() returned "));
        Serial.println(err.f_str());
      }
    }

    Serial.println(F("Error reading configs"));
    delay(6000);
    ESP.restart();
  }

} config;

#endif