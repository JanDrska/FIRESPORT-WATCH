//####### PROGRAM HASICSKE STOPKY  ######
// PROGRAM VYTVORIL JAN DRSKA PRO JEDNOTKU SDH JIZBICE
/*
              /----/  /----/        /----/  /----/
             /    /  /    /  /--/  /    /  /    /
            /----/  /----/        /----/  /----/
           /    /  /    /  /--/  /    /  /    /
          /----/  /----/        /----/  /----/
*/

// VERZE 2.0
// POSLEDNI UPRAVA 03.04.2022

#include "sdhTimer.h"
#include "sdhDisplay.h"

#include <PCF8574.h>
#include <SPI.h>
#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>

#define TLA 2      //tlačítko A - START
#define TLB 3      //tlačítko B - RESET
#define TLProg 8   //prepinac programu
#define ODPOCET_TERC 300000  //pozadovany cas v ms (300 000 je 5 minut)

TimerData timerL;
TimerData timerR;
DisplaySdh display; 
RF24 radio(9, 10);  // CE, CSN

const byte addresses [] [6] = {"00001","00002"} ; // adresa komunikatoru
bool terc1, terc2, prazdno = LOW;    //terc 1 - LEVY a 2 - PRAVY
bool init_delivery_confirm_bit = LOW;
bool READY = LOW;
const char text1[] = "INIT";
const char text2[] = "RESET";
char text [32] = "";

int STOP1 = 0;
int STOP2 = 0;
int program =0;

bool startCmd = false;
bool stopCmd = false;
bool resetCmd = false;
bool vypustitCmd = false;

void choose_program(); // funkce na vyber programu
void start();

void setup() 
{
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setRetries(4,10);
  radio.setPALevel(HIGH);
  radio.startListening();

  pinMode(TLA, INPUT_PULLUP);        //start
  pinMode(TLB, INPUT_PULLUP);        //stop
  pinMode(TLProg, INPUT_PULLUP);     //prepinac programu (1-stopky 0- odpocet)

  //Serial.println("CASOMIRA SDH JIZBICE");
  
  display.init();  //inicializace 7segmentovek
  choose_program();

  READY = true;
}

void loop() 
{
  switch (program)
  {
    case 1: //######## STOPKY POZARNI SPORT ########
      
      timerL.casSTART = 1;
      if (radio.available())
      {
        if(radio.available())
        {
          radio.read(&text, sizeof(text));
          String ZPRAVA = String(text);
          if (ZPRAVA == "PRIPRAVEN") 
          {
            READY = HIGH;
            Serial.print("A");
          }
        }
      }
      if(Serial.available())
      {
        char txt = Serial.read();
        switch(txt)
        {
          case 'A': // START
            startCmd = true;           
            break;
          case 'B': // STOP   
            stopCmd = true;        
            break;
          case 'C': // RESET
            resetCmd = true;
            break;
          case 'D': // VYPUSTIT
            vypustitCmd = true;
            break;
        }
      }
  
      if(startCmd && READY) //při stisknutí start...
        start();        //skoč na start
        
      while(timerL.casSTART > 1)   //pokud bylo odstartováno pak
      {
        if(Serial.available())
      {
        char txt = Serial.read();
        switch(txt)
        {
          case 'B': // STOP   
            stopCmd = true;        
            break;
          case 'C': // RESET
            resetCmd = true;
            break;
        }
      }
        if(STOP1 == 0)    //průběžný čas prvního terče
        {
          timerL.Time();
          display.sendData(timerL,timerR);        
        }
              
        if(STOP2 == 0)    //průběžný čas 2.terče
        {
          timerR.Time();
          display.sendData(timerL,timerR);
        }

        if(terc1 == HIGH) //konec 1.terče
        {
          STOP1 = 1;
          timerL.stopTimming();
          display.sendData(timerL,timerR);
          //timerL.sendDataSerial('L');
          terc1 = LOW;
        }
    
        if(terc2 == HIGH) //konec 2. terče
        {
          STOP2 = 1;
          timerR.stopTimming();
          display.sendData(timerL,timerR);
          //timerR.sendDataSerial('R');
          terc2 = LOW; 
        }
    
        if(resetCmd) // prikaz pro reset
        {
          timerL.casSTART = 1;
          display.init();
          //choose_program();
          resetCmd = false;
          startCmd = false;
          stopCmd = false;
          radio.stopListening();

          if(radio.write(&text2, sizeof(text2)))
          {
            //Serial.println("RESET TERCE");
          } 
        }

        if(stopCmd)
        {
          STOP1 = true;
          STOP2 = true;
          terc1 = true;
          terc2 = true;
          stopCmd = false;;
        }


        if(radio.available())
        {
          char text [32] = {0};
          radio.read(&text,sizeof(text));
          Serial.println(text);
        
          String ZPRAVA = String(text);
          if (ZPRAVA == "LEVY")
          {
            terc1 = HIGH;
            Serial.print("C");
          } 
          if (ZPRAVA == "PRAVY") 
          {
            terc2 = HIGH;
            Serial.print("B");
          }
          if (ZPRAVA == "PRAZDNO") 
          {
            prazdno = HIGH;
            Serial.print("D");
          }
      
          radio.writeAckPayload(1, &text, sizeof(text));
        }
      }
    
     break;

    case 2: // ######## ODPOCET ########
             
      timerR.casSTART = 1;
    
      if(digitalRead(TLA) == LOW) //při stisknutí start...
        start();        //skoč na start
        
      while(timerR.casSTART > 1)   //pokud bylo odstartováno pak
      {         
        timerR.casODPOCET =timerR.casSTART-millis()+ODPOCET_TERC;
        timerR.casTERC_M = timerR.casODPOCET / 60000;  //výpočet minut
        timerR.casTERC_S = timerR.casODPOCET % 60000;  //výpočet sekund
        timerR.casTERC_S = timerR.casTERC_S / 1000;     //výpočet sekund
        timerR.casTERC_ms = timerR.casODPOCET % 1000;  //výpočet ms
    
        if((timerL.casTERC_M==0)&&(timerL.casTERC_S==0))
        {
          program=1;
          timerL.casTERC_ms=0;
          display.reset();
          break;               
        }

        if(digitalRead(TLB) == LOW) //při stisknutí B restart
        {
          timerR.casSTART = 1; 
          display.reset();
          break;
        }
      }
    case 6: // ERROR - RESET NUTNY
        break;
   break; 
  }
}

void start()
{
  timerL.startTimming();
  timerR.startTimming();
  radio.startListening(); // zapnuti prijmu radia
  startCmd = false;
}

void choose_program()
{
  /*
  if(digitalRead(TLProg)== HIGH)program=1;
  else
  {*/
    program=1;
    start();  
  //}
}
