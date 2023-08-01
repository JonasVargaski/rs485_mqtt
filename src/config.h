#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_
#include "Arduino.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <vector>
#include "utils.h"

const size_t JSON_SIZE = JSON_OBJECT_SIZE(50) + 2048;

#define LED_COLOR_OK 0, 0, 255, 0                   // VERDE
#define LED_COLOR_MODBUS_ERROR 0, 0, 0, 255         // AZUL
#define LED_COLOR_SERVER_ERROR 0, 214, 37, 152      // ROSA
#define LED_COLOR_WIFI_ERROR 0, 255, 0, 0           // VERMELHO
#define LED_COLOR_CONFIGURATION_MODE 0, 255, 255, 0 // AMARELO

enum ModbusStatus
{
  MB_STATUS_IDLE,
  MB_STATUS_PUBLISH,
};

struct AppConfig
{
  String deviceId = "485Gateway-" + hexToStr(ESP.getEfuseMac());

  struct WifiConfig
  {
    char ssid[30] = "";
    char pass[30] = "";
    IPAddress apIP = IPAddress(192, 168, 4, 1);
    IPAddress netMsk = IPAddress(255, 255, 255, 0);
    bool is_configuration_mode = false;
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
    int slaveId = 1;
    int recordsPerRead = 35;
    ModbusStatus status = MB_STATUS_IDLE;
    std::vector<u_int16_t> holdingRegs;
    std::vector<u_int16_t> coilRegs;
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
        modbus.holdingRegs.resize(json[F("modbus")][F("holdings")].as<int>() | 0);
        modbus.coilRegs.resize(json[F("modbus")][F("coils")].as<int>() | 0);

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

  bool save(FS &fs)
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
        json["wifi"]["ssid"] = wifi.ssid;
        json["wifi"]["pass"] = wifi.pass;

        // mqtt
        json["mqtt"]["server"] = mqtt.server;
        json["mqtt"]["port"] = mqtt.port;
        json["mqtt"]["interval"] = mqtt.interval;

        // modbus
        //  json["modbus"]["baudrate"] = modbus.baudrate;
        //  json["modbus"]["holdings"] = modbus.holdingRegs.size();
        //  json["modbus"]["coils"] = modbus.coilRegs.size();

        File configFile = fs.open("/config.json", "w");
        serializeJson(json, configFile);

        Serial.println(F("Configuration updated success!"));
        serializeJson(json, Serial);
        configFile.close();
        Serial.println("");
        return true;
      }
      else
      {
        Serial.print(F("Error: deserializeJson() returned "));
        Serial.println(err.f_str());
      }
    }

    Serial.println(F("Error writing configs"));
    return false;
  }

} config;

#endif