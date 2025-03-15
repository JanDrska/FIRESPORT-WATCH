
/*
#include <ModbusRTUSlave.h>

#define MODBUS_SERIAL Serial
#define MODBUS_BAUD 115200
#define MODBUS_CONFIG SERIAL_8N1
#define MODBUS_UNIT_ID 2

const int16_t DE_RE = 2;

ModbusRTUSlave modbus(MODBUS_SERIAL, DE_RE);

const uint8_t numDiscreteInputs = 2;
const uint8_t numHoldingRegisters = 2;
const uint8_t numInputRegisters = 2;

unsigned long lastTime = 0;

bool discreteInputs[numDiscreteInputs];
uint16_t holdingRegisters[numHoldingRegisters];
uint16_t inputRegisters[numInputRegisters];

void setup() 
{
  pinMode(DE_RE, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(13, OUTPUT);
  
  modbus.configureDiscreteInputs(discreteInputs, numDiscreteInputs);
  modbus.configureHoldingRegisters(holdingRegisters, numHoldingRegisters);
  modbus.configureInputRegisters(inputRegisters, numInputRegisters);

  MODBUS_SERIAL.begin(MODBUS_BAUD, MODBUS_CONFIG);
  modbus.begin(MODBUS_UNIT_ID, MODBUS_BAUD, MODBUS_CONFIG);
}

void loop() 
{
  // read inputs

  inputRegisters[0] = 10;
  inputRegisters[1] = 15;
  discreteInputs[0] = 1;
  discreteInputs[1] = 0;

  // communication routine
  modbus.poll();

  // write outputs
  digitalWrite(4, (holdingRegisters[0] >> 0) & 1);
  digitalWrite(5, (holdingRegisters[0] >> 1) & 1);
  digitalWrite(6, (holdingRegisters[0] >> 2) & 1);
  digitalWrite(7, (holdingRegisters[0] >> 3) & 1);

  // blink led
  if((millis() - lastTime) >= 100)
  {
    lastTime = millis();
    if(digitalRead(13))
      digitalWrite(13,0);
    else
      digitalWrite(13, 1);
  }
}
*/