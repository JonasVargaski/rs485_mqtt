#include <Arduino.h>
#include <ModbusRTU.h>
#include <SoftwareSerial.h>

int serialBaud = 9600;
int serialconfig = 3;

SoftwareSerial softSerial(4, 5);
ModbusRTU mb;

void setup()
{
  Serial.begin(115200, SERIAL_8N1);
  softSerial.begin(serialBaud, (Config)serialconfig);
  mb.begin(&softSerial);

  mb.slave(1);

  mb.addHreg(0);
  mb.Hreg(0, 100);
}

void loop()
{
  mb.task();
  yield();
}