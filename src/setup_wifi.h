#ifndef _APP_WIFI_SETUP_H_
#define _APP_WIFI_SETUP_H_
#include "utils.h"
#include "config.h"

void setupWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(config.deviceId.c_str());
  WiFi.begin(config.wifi.ssid, config.wifi.pass);
  WiFi.softAPConfig(config.wifi.apIP, config.wifi.apIP, config.wifi.netMsk);
  WiFi.softAP(config.deviceId.c_str(), hexToStr(ESP.getEfuseMac()));

  Serial.print(F("Connecting to ssid: "));
  Serial.println(config.wifi.ssid);
}

#endif