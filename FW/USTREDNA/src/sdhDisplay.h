#if !defined(sdhDisplay)
#define sdhDisplay

#include <PCF8574.h>
#include "sdhTimer.h"

class DisplaySdh
{
private:

  // dp ABCGFED

  byte cislice[10] = 
  {B01110111,B00110000, // 0,1
  B01101011,B01111001, // 2,3 
  B00111100,B01011101, // 4,5 
  B01011111,B01110000, // 6,7 
  B01111111,B01111101};// 8,9 

  byte znak[5] = 
  {B00001000,     // '-'
  B01110111,      // 'D'
  B00111110,      // 'H'
  B01000111,      // 'C'
  B01101100};     // '°'

  byte I2C_address[8]= 
  {0x20,0x21,           //prvni a druhy modul
  0x25,0x24,            //treti a ctvrty modul
  0x22,0x23,            //paty a sesty modul
  0x26,0x27};           //sedmy a osmy modul

  int splitTime[2] = {0};

public:

  void init();
  void initGraphical();
  void reset();
  void sendData(TimerData data1, TimerData data2);
  void write8(int val, char flag);  // flag - 'c' pro zapsani znaku, ostatni pro cislo
  void I2C_write(int num,int address);
  void split(int* DaM, int cas);  //funkce na rozdeleni dvoucifernych cisel

};

#endif // sdhDisplay