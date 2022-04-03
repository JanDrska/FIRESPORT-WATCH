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

void TimerData::startTimming()
{
  casSTART = millis();
}

void TimerData::stopTimming()
{
  casTERC_end = millis() - casSTART;
  casTERC_M = casTERC_end / 60000;  //výpočet minut
  casTERC_S = casTERC_end % 60000;  //výpočet sekund
  casTERC_S = casTERC_S / 1000;     //výpočet sekund
  casTERC_ms = casTERC_end % 1000;  //výpočet ms
}

void TimerData::Time()
{
  casTERC = millis() - casSTART;
  casTERC_M = casTERC / 60000;
  casTERC_S = (casTERC % 60000)/1000;
  casTERC_ms = casTERC % 1000;
}

void TimerData::sendDataSerial(char name)
{
  Serial.print("TERC ");
  Serial.print(name);
  Serial.print(" : ");
  Serial.print(casTERC_M);
  Serial.print(" : ");          
  Serial.print(casTERC_S);
  Serial.print(" : ");      
  Serial.print(casTERC_ms);
  Serial.println("   ");
}