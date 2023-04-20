#include <Arduino.h>
#include <ModbusRTU.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

#define SLAVE_ID 1
#define FIRST_REG 0
#define REG_COUNT 3

String clientId = "hauhasuahsuhasuhas";
WiFiClient wifi;

PubSubClient client(wifi);
SoftwareSerial softSerial(4, 5);
ModbusRTU mb;

const char *mqtt_server = "broker.mqtt-dashboard.com";
const int mqtt_port = 1883;
const char *topic_command = "esp32/command";
unsigned long myTime;

// from json
int serialBaud = 9600;
int serialconfig = 3;
const char *wifi_ssid = "Vargaski";
const char *wifi_pass = "12345678";

bool cb(Modbus::ResultCode event, uint16_t transactionId, void *data)
{ // Callback to monitor errors
  if (event != Modbus::EX_SUCCESS)
  {
    Serial.print("Request result: 0x");
    Serial.print(event, HEX);
  }
  return true;
}

void callback(char *topic, byte *message, unsigned int length)
{
  return;
}

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname("EspMqttHub");
  WiFi.begin(wifi_ssid, wifi_pass);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.print("Failed to connect");
    delay(5000);
    ESP.restart();
  }
  Serial.print("Ready!\nIP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
}

int converterSinalWifi()
{
  return round(constrain(map(WiFi.RSSI(), -100, -60, 0, 100), 0, 100) / 10.0) * 10;
}

void setup()
{
  Serial.begin(115200, SERIAL_8N1);
  Serial.println("Booting");
  initWiFi();

  ArduinoOTA
      .onStart([]() {})
      .onEnd([]() {})
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               { Serial.printf("Error[%u]: ", error); });

  softSerial.begin(serialBaud, (Config)serialconfig);
  mb.begin(&softSerial);
  mb.master();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  ArduinoOTA.begin();
}

void waitTillModBusDone()
{
  while (mb.slave())
  {
    mb.task();
    yield();
  }
}

void loop()
{
  if (!client.connected())
  {
    client.connect(clientId.c_str());
  }

  if (!mb.slave() && (millis() - myTime) > 1000)
  {
    myTime = millis();
    uint16_t res1 = 0;
    uint16_t res2 = 0;
    uint16_t res3 = 0;
    uint16_t res4 = 0;

    mb.writeHreg(SLAVE_ID, 2, WiFi.status());
    waitTillModBusDone();
    mb.writeHreg(SLAVE_ID, 3, converterSinalWifi());
    waitTillModBusDone();

    mb.readHreg(SLAVE_ID, 0, &res1, 1, cb);
    waitTillModBusDone();
    mb.readHreg(SLAVE_ID, 1, &res2, 1, cb);
    waitTillModBusDone();
    mb.readHreg(SLAVE_ID, 2, &res3, 1, cb);
    waitTillModBusDone();
    mb.readHreg(SLAVE_ID, 3, &res4, 1, cb);
    waitTillModBusDone();

    char buffer[70];
    sprintf(buffer, "{\"40001\":%d, \"40002\":%d, \"40003\":%d, \"40004\":%d}", res1, res2, res3, res4);
    client.publish(topic_command, buffer);
    Serial.println(buffer);
  }

  mb.task();
  yield();
  client.loop();
  ArduinoOTA.handle();
}