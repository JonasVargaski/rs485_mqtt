#ifndef _APP_WEB_SERVER_H_
#define _APP_WEB_SERVER_H_
#include "Arduino.h"
#include <DNSServer.h>
#include <AsyncTCP.h>
#include "ArduinoJson.h"
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "config.h"

AsyncWebServer webServer(80);
DNSServer dnsServer;

void setupWebServer(void);
void notFound(AsyncWebServerRequest *request);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, F("text/plain"), F("Not found"));
}

class HomeRequestHandler : public AsyncWebHandler
{
public:
  HomeRequestHandler() {}
  virtual ~HomeRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request)
  {
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request)
  {
    request->redirect("/");
  }
};

void setupWebServer()
{
  Serial.print(F("\nserver IP on: "));
  Serial.println(config.wifi.apIP.toString());

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", config.wifi.apIP);

  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(SPIFFS, F("/index.html"), F("text/html; charset=utf-8")); });

  webServer.on("/api/config", HTTP_GET, [](AsyncWebServerRequest *request)
               {
   File file = SPIFFS.open(F("/config.json"), "r");
    request->send(200,F("application/json"), file.readString());
      file.close(); });

  webServer.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
               {
      bool ok = false;
      int params = request->params();
      for (int i=0; i<params; i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          if (p->name() == "wifi.ssid")
            strlcpy(config.wifi.ssid, p->value().c_str(), sizeof(config.wifi.ssid));
          if (p->name() == "wifi.pass")
            strlcpy(config.wifi.pass, p->value().c_str(), sizeof(config.wifi.pass));
          if (p->name() == "mqtt.server")
            strlcpy(config.mqtt.server, p->value().c_str(), sizeof(config.mqtt.server));
          if (p->name() == "mqtt.port")
            config.mqtt.port = p->value().toInt();
          if (p->name() == "mqtt.interval")
            config.mqtt.interval = p->value().toInt();
          
          ok = config.save(SPIFFS); 
        }
      }
      request->send(200, F("text/html"), ok ? F("Success!") : F("Error!"));
       if(ok){
          delay(2000);
          ESP.restart();
       } });

  webServer.addHandler(new HomeRequestHandler()).setFilter(ON_AP_FILTER);
  webServer.onNotFound(notFound);
  webServer.begin();
}

#endif