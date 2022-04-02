# include "sdhTimer.h"
# include <Arduino.h>

void TimerData::init()
{
  casSTART = 0;       //startovní čas
  casODPOCET = 0;     //delka odpoctu
  casTERC = 0;        //průběžný čas 1. terče
  casTERC_end = 0;    //konečný čas 1. terče
  casTERC_M = 0;      //konečný čas 1. terče - minuty
  casTERC_S = 0;       //konečný čas 1. terče - sekundy
  casTERC_ms = 0;      //konečný čas 1. terče - ms
}

void TimerData::sendDataSerial()
{
  Serial.print("TERC L: ");
  Serial.print(casTERC_M);
  Serial.print(" : ");     
            
  Serial.print(casTERC_S);
  Serial.print(" : ");
        
  Serial.print(casTERC_ms);
  Serial.println("   ");
}