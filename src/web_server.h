#ifndef _APP_WEB_SERVER_H_
#define _APP_WEB_SERVER_H_
#include "Arduino.h"
#include <DNSServer.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "config.h"

AsyncWebServer webServer(80);
DNSServer dnsServer;

void setupWebServer(void);
void notFound(AsyncWebServerRequest *request);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, F("text/plain"), F("Not found"));
}

class CaptiveRequestHandler : public AsyncWebHandler
{

public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request)
  {
    // request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request)
  {
    AsyncResponseStream *response = request->beginResponseStream(F("text/html"));
    response->print(F("<!DOCTYPE html><html><head><title>Captive Portal</title></head><body>"));
    response->print(F("<p>This is out captive portal front page.</p>"));
    response->printf("<p>You were trying to reach: http://%s%s</p>", request->host().c_str(), request->url().c_str());
    response->printf("<p>Try opening <a href='http://%s'>this link</a> instead</p>", WiFi.softAPIP().toString().c_str());
    response->print(F("</body></html>"));
    request->send(response);
  }
};

void setupWebServer()
{

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", config.wifi.apIP);

  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(200, F("text/plain"), F("Hello, world")); });

  webServer.onNotFound(notFound);
  webServer.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only
  webServer.begin();
}

#endif