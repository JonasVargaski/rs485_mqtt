#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_
#include "Arduino.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "utils.h"

const size_t JSON_SIZE = JSON_OBJECT_SIZE(50) + 2048;
#define MB_MAX_REGISTERS 50

struct ModbusMapReg
{
  char type = 'N';
  int slave_id = 0;
  uint16_t addr = 0;
  uint16_t h_value = 0;
  bool c_value = false;
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
    char server[60] = "broker.mqtt-dashboard.com";
    char clientId[60] = "";
    char resultTopic[60] = "";
    unsigned int port = 1883;
    unsigned int interval = 1000;
  } mqtt;

  struct ModbusConfig
  {
    int baudrate = 9600;
    int serialType = 3;
    int reg_count = 0;
    ModbusMapReg registers[MB_MAX_REGISTERS];
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
        strlcpy(mqtt.clientId, json[F("mqtt")][F("clientId")] | mqtt.clientId, sizeof(mqtt.clientId));
        strlcpy(mqtt.resultTopic, json[F("mqtt")][F("resultTopic")] | mqtt.resultTopic, sizeof(mqtt.resultTopic));
        mqtt.interval = json[F("mqtt")][F("interval")].as<int>() | mqtt.interval;
        mqtt.port = json[F("mqtt")][F("port")].as<int>() | mqtt.port;

        // modbus
        modbus.baudrate = json[F("modbus")][F("baudrate")].as<int>() | modbus.baudrate;
        modbus.serialType = json[F("modbus")][F("serialType")].as<int>() | modbus.serialType;

        JsonArray mapArray = json["modbus"]["map"].as<JsonArray>();
        for (JsonObject regMap : mapArray)
        {
          if (modbus.reg_count >= MB_MAX_REGISTERS)
            return;

          ModbusMapReg mb_reg;
          char type[2] = "";
          strlcpy(type, regMap[F("type")] | "", sizeof(type));
          if (type[0] == 'H' | type[0] == 'C')
          {
            modbus.registers[modbus.reg_count].type = type[0];
            modbus.registers[modbus.reg_count].addr = regMap["addr"].as<int>();
            modbus.registers[modbus.reg_count].slave_id = regMap["sId"].as<int>();
            modbus.reg_count += 1;
          }
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