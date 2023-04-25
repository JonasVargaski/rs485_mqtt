#ifndef _APP_UTILS_H_
#define _APP_UTILS_H_
#include "Arduino.h"

String hexToStr(const unsigned long &h, const byte &l = 8)
{
  String s;
  s = String(h, HEX);
  s.toUpperCase();
  s = ("00000000" + s).substring(s.length() + 8 - l);
  return s;
}

void toggleBuiltInLed()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

#endif