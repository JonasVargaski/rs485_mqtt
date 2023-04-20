#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_
#include "Arduino.h"
#include "utils.h"

const size_t JSON_SIZE = JSON_OBJECT_SIZE(20) + 4048;

struct AppConfig
{
  String deviceId = "Mqtt485Gateway-" + hexToStr(ESP.getEfuseMac());

  struct WifiConfig
  {
    char ssid[30] = "";
    char pass[20] = "";
    IPAddress apIP = IPAddress(192, 168, 4, 1);
    IPAddress netMsk = IPAddress(255, 255, 255, 0);
  } wifi;

  struct MqttConfig
  {
    char server[60] = "broker.mqtt-dashboard.com";
    char clientId[50] = "";
    char modbusTopic[50] = "";
    unsigned int port = 1883;
    unsigned int publishInterval = 1000;
  } mqtt;

  struct ModbusConfig
  {
    int slaveId = 1;
    int serialType = 3;
    int baudrate = 9600;
    int hreg[30] = {0, 1, 2};
    int coils[20];
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
        strlcpy(wifi.ssid, json[F("wifi")][F("ssid")] | wifi.ssid, sizeof(wifi.ssid));
        strlcpy(wifi.pass, json[F("wifi")][F("pass")] | wifi.pass, sizeof(wifi.pass));

        strlcpy(mqtt.server, json[F("mqtt")][F("server")] | mqtt.server, sizeof(mqtt.server));
        strlcpy(mqtt.clientId, json[F("mqtt")][F("clientId")] | mqtt.clientId, sizeof(mqtt.clientId));
        strlcpy(mqtt.modbusTopic, json[F("mqtt")][F("modbusTopic")] | mqtt.modbusTopic, sizeof(mqtt.modbusTopic));

        return;
      }
    }
    Serial.println(F("Error reading configs"));
  }

  void save(FS &fs)
  {
    Serial.println(F("SAVE CONFIGS"));
  }
} config;

#endif