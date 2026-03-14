#include "sdhDisplay.h"

uint8_t DisplaySdh::init()
{
  for(size_t i=0;i<=1;i++)
  {
    for(size_t i =0;i<8;i++)
    {
      PCF8574 pcf(I2C_address[i]);
      if(!pcf.begin())
        return false;
      pcf.write8(cislice[8]);
      //delay(250);
      }
    }

    delay (1000);

  for(size_t i=0;i<=1;i++)
  {
    for(size_t i =0;i<8;i++)
    {
      PCF8574 pcf(I2C_address[i]);
      if(!pcf.begin())
        return false;
      pcf.write8(znak[0]);
      //delay(250);
    }
  }

  return true;
}

void DisplaySdh::initGraphical()
{
    // TODO nabeh dokola
}

void DisplaySdh::reset()
{
  for(size_t i=0;i<=1;i++)
  {
    for(size_t i =0;i<8;i++)
    {
      PCF8574 pcf(I2C_address[i]);
      pcf.begin();
      pcf.write8(cislice[0]);
    }
  }
}
/*
void DisplaySdh::sendData(TimerData data1, TimerData data2)
{
//########################## MINUTY L ##############################
  split(splitTime,data1.casTERC_M);        
  I2C_write(splitTime[0],I2C_address[0]);
  I2C_write(splitTime[1],I2C_address[1]);
            
//########################## SEKUNDY L ##############################
  split(splitTime,data1.casTERC_S);         
  I2C_write(splitTime[0],I2C_address[2]);
  I2C_write(splitTime[1],I2C_address[3]);        

//########################## MINUTY R ##############################
  split(splitTime,data2.casTERC_M);            
  I2C_write(splitTime[0],I2C_address[4]);
  I2C_write(splitTime[1],I2C_address[5]);

//########################## SEKUNDY R ##############################         
  split(splitTime,data2.casTERC_S);            
  I2C_write(splitTime[0],I2C_address[6]);
  I2C_write(splitTime[1],I2C_address[7]);
}
*/

void DisplaySdh::sendData(TimerData data1, TimerData data2)
{
//########################## SEKUNDY ##############################
  split2(splitTime,data1.casTERC_S);        
  I2C_write(splitTime[0],I2C_address[0]);
  I2C_write(splitTime[1],I2C_address[1]);
            
//########################## MILISEKUNDY L ##############################
  split3(splitTime,data1.casTERC_ms);         
  I2C_write(splitTime[1],I2C_address[2]);
  I2C_write(splitTime[2],I2C_address[3]);        

//########################## SEKUNDY R ##############################
  split2(splitTime,data2.casTERC_S);            
  I2C_write(splitTime[0],I2C_address[4]);
  I2C_write(splitTime[1],I2C_address[5]);

//########################## MILISEKUNDY R ##############################         
  split3(splitTime,data2.casTERC_ms);            
  I2C_write(splitTime[1],I2C_address[6]);
  I2C_write(splitTime[2],I2C_address[7]);
}

void DisplaySdh::write8(int val,char flag)
{
  for(size_t i =0;i<8;i++)
  {
    PCF8574 pcf(I2C_address[i]);
    pcf.begin();
    if (flag == 'c')
      pcf.write8(znak[val]);
    else
      pcf.write8(cislice[val]);
  }
}

void DisplaySdh::I2C_write(int num,int address)
{
    PCF8574 pcf(address);
    pcf.write8(cislice[num]);
}

void DisplaySdh::split2(int* DaM, int cas)  //funkce na splitTime dvoucifernych cisel
{
  DaM[0]= cas % 10; //jednotky
  if(((cas - DaM[0])/10)>=0)DaM[1] = (cas - DaM[0])/10; //desitky
  //else DaM[1];
}
void DisplaySdh::split3(int* DaM, int cas)
{
    int j = cas % 10;
    int d = (cas / 10) % 10;
    int s = cas / 100;

    if (cas < 10)           // jednociferné
    {
        DaM[2] = j;
        DaM[1] = 0;
        DaM[0] = 0;
    }
    else if (cas < 100)     // dvouciferné
    {
        DaM[2] = d;
        DaM[1] = j;
        DaM[0] = 0;
    }
    else                    // trojciferné
    {
        DaM[2] = s;
        DaM[1] = d;
        DaM[0] = j;
    }
}
