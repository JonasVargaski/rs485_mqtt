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

int getRSSIInPercent()
{
  return round(constrain(map(WiFi.RSSI(), -100, -60, 0, 100), 0, 100) / 10.0) * 10;
}

void toggleBuiltInLed()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

String ipToStr(const IPAddress &ip)
{
  String res = "";
  for (byte bFn = 0; bFn < 3; bFn++)
  {
    res += String((ip >> (8 * bFn)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

template <class T, size_t N>
constexpr size_t len(const T (&)[N]) { return N; }

#endif