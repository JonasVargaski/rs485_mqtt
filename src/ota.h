#ifndef _APP_OTA_H_
#define _APP_OTA_H_
#include "Arduino.h"
#include <ArduinoOTA.h>

void setupOTA()
{
  ArduinoOTA
      .onStart([]() {})
      .onEnd([]() {})
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               { Serial.printf("Error[%u]: ", error); });

  ArduinoOTA.begin();
}

#endif