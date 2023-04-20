#ifndef _APP_MODBUS_H_
#define _APP_MODBUS_H_
#include <ModbusRTU.h>
#include <SoftwareSerial.h>
#include "Arduino.h"
#include "config.h"

/*** Function definition; */
void setupModbusMaster(void);
bool modbusCallback(Modbus::ResultCode event, uint16_t transactionId, void *data);
void modbusWaitTillDone(void);
/*** End Function definition; */

SoftwareSerial softSerial(4, 5);
ModbusRTU modbus;

void setupModbusMaster()
{
  softSerial.begin(config.modbus.baudrate, (Config)config.modbus.serialType);
  modbus.begin(&softSerial);
  modbus.master();
}

bool modbusCallback(Modbus::ResultCode event, uint16_t transactionId, void *data)
{
  if (event != Modbus::EX_SUCCESS)
  {
    Serial.print(F("Modbus request error: 0x"));
    Serial.println(event, HEX);
  }
  return true;
}

void modbusWaitTillDone()
{
  while (modbus.slave())
  {
    modbus.task();
    yield();
  }
}

#endif