#ifndef _APP_WEB_SERVER_H_
#define _APP_WEB_SERVER_H_
#include "Arduino.h"
#include <DNSServer.h>
#include <AsyncTCP.h>
#include "ArduinoJson.h"
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

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", config.wifi.apIP);

  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(SPIFFS, F("/index.html"), F("text/html; charset=utf-8")); });

  webServer.on("/api/config", HTTP_GET, [](AsyncWebServerRequest *request)
               {
   File file = SPIFFS.open(F("/config.json"), "r");
    request->send(200,F("application/json"), file.readString());
      file.close(); });

  webServer.on(
      "/upload", HTTP_POST, [](AsyncWebServerRequest *request)
      { request->redirect("/"); },
      [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
      {
        if (!index)
        {
          if (filename != "config.json")
          {
            Serial.println(F("Invalid file name"));
            return;
          }

          Serial.printf("UploadStart: %s\n", filename.c_str());
          SPIFFS.remove(F("/config.json"));
          File file = SPIFFS.open(F("/config.json"), "w");
          if (file)
            file.write(data, len);

          if (final)
          {
            Serial.printf("UploadEnd: %s, %u B\n", F("/config.json"), index + len);
            file.close();
            ESP.restart();
          }
        }
      });

  webServer.addHandler(new HomeRequestHandler()).setFilter(ON_AP_FILTER);
  webServer.onNotFound(notFound);
  webServer.begin();
}

#endif