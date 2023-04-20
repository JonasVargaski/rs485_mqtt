#ifndef _APP_MQTT_H_
#define _APP_MQTT_H_
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

/*** Function definition; */
void setupMqtt(void);
void reconnectMqtt(void);
void mqttCallback(char *topic, byte *message, unsigned int length);
/*** End Function definition; */

WiFiClient wifi;
PubSubClient mqtt(wifi);

void mqttCallback(char *topic, byte *message, unsigned int length)
{
  return;
}

void setupMqtt()
{
  mqtt.setServer(config.mqtt.server, config.mqtt.port);
  mqtt.setCallback(mqttCallback);
}

void reconnectMqtt(void)
{
  if (!mqtt.connected() && (WiFi.status() == WL_CONNECTED || WiFi.status() == WL_IDLE_STATUS))
  {
    mqtt.connect(config.mqtt.clientId);
  }
}
#endif