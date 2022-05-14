//####### PROGRAM PRO OVLADACI PULT HASICSKYCH STOPEK  ######
// PROGRAM VYTVORIL JAN DRSKA PRO JEDNOTKU SDH JIZBICE
// PROGRAM JE VYHRADNIM VLASTNICTVIM AUTORA A SDH JIZBICE - JAKEKOLIV SIRENI PROGRAMU BEZ VEDOMI AUTORA JE ZAPOVEZENO!!!
/*
              /----/  /----/        /----/  /----/
             /    /  /    /  /--/  /    /  /    /
            /----/  /----/        /----/  /----/
           /    /  /    /  /--/  /    /  /    /
          /----/  /----/        /----/  /----/
*/

// VERZE 1.0
// POSLEDNI UPRAVA 23.01.2022

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(2, 3); // RX, TX 

#include <Wire.h> 
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
#include <sdhTimer.h>
#include "ledControl.hpp"
LiquidCrystal_I2C lcd(0x3F,16,2);  

int counter = 0; 
int page=1;
int Ready=0;
int submenu=0;
bool pushed = 0;
int aState;
int aLastState; 
int last_counter=0; 
bool started = false;
unsigned long int actTime, prevTime = 0;
unsigned long int ButtonActTime, ButtonPrevTime = 0;
bool timerReady = false;

int error = 0;
bool endR, endL = false;
bool button;

TimerData timerL;
TimerData timerR;
LED ledReady;

uint8_t arrow[8] = {0x00, 0x04 ,0x06, 0x1f, 0x06, 0x04, 0x00};

// Externi klavesnice
#define E_Start A0
#define E_Stop A1
#define E_Reset A2
#define E_Enable A3

// Enkoder
#define ENC_BTN 2
#define OutputA 8
#define OutputB 9

// Externi vstupy start
#define EXT_IN1 3 // moznost vyuziti preruseni
#define EXT_IN2 4

// Indikacni LED

#define LED_READY 7
#define LED_R 10
#define LED_L 13
#define LED_EMPTY 11
#define LED_RUN 12

/// text1 - 1. radek, text2 - 2. radek, buttonPushed - promenna pro nulovani promenne pushed, position - pozice sipky (1 - UP; 0 - DOWN; 2 - NIC), waitMs - delay
void LcdMenuWrite (String text1,String text2,bool buttonPushed,int position,unsigned long int waitMs = 0); 
void ClearMenuData();
bool SendMessageSerial(String Msg);
void PrintTime(TimerData data, bool id);



void setup()
{
  // Externi klavesnice
  pinMode(E_Start, INPUT);
  pinMode(E_Stop, INPUT);
  pinMode(E_Reset, INPUT);
  pinMode(E_Enable, INPUT);

  // Enkoder
  pinMode (ENC_BTN,INPUT_PULLUP);     
  pinMode (OutputA,INPUT);
  pinMode (OutputB,INPUT);

  // Externi vstupy start
  pinMode (EXT_IN1,INPUT);
  pinMode (EXT_IN2,INPUT);

  // Indikacni LED
  pinMode (LED_READY, OUTPUT);
  pinMode (LED_R, OUTPUT);
  pinMode (LED_L, OUTPUT);
  pinMode (LED_EMPTY, OUTPUT);
  pinMode (LED_RUN, OUTPUT);
  
  // VCC PINY - NEBUDE TU PAK !
  pinMode (5,OUTPUT);
  digitalWrite(5,HIGH);
  pinMode (6,OUTPUT);
  digitalWrite(6,HIGH);

  aLastState = digitalRead(OutputA);  
  
  lcd.init();                
  lcd.backlight();            
  lcd.createChar(1, arrow);  
  lcd.home();                 
  
  Serial.begin(9600);  

  LcdMenuWrite("  SDH JIZBICE","       V1",pushed,1,1000);
  LcdMenuWrite("Vyber sport","Pozarni sport",pushed,0);

  //mySerial.begin(9600);

}

void loop()
{ 
  if(timerReady)
    digitalWrite(LED_READY,ledReady.blink(500));
  else
    digitalWrite(LED_READY,LOW);

  if (started)
  { 
    digitalWrite(LED_EMPTY,LOW);
    timerL.Time();
    timerR.Time();
    if(!endL&&!button)
      PrintTime(timerL,0);
    if(!endR&&!button)
      PrintTime(timerR,1);
    if(endL&&endR)
    {
      started = false;
      endL = false;
      endR = false;
    }
    ButtonActTime = millis();

    if(!digitalRead(ENC_BTN))
      button = true;

    if(ButtonActTime - ButtonPrevTime > 7000)
    {
      button = false;
      ButtonPrevTime = ButtonActTime;
    }
  }

  actTime = millis();

  if(actTime - prevTime > 100) // kazdych 50 ms kontrola prijatych zprav - RESET,INIT apod. 
  {
    if(Serial.available())
    {
      char txt = Serial.read();
      //Serial.print(txt);

      switch(txt)
      {
        case 'A':
          digitalWrite(LED_READY,HIGH);
          break;
        case 'B':
          digitalWrite(LED_R,HIGH);
          timerR.stopTimming();
          endR = true;
          break;
        case 'C':
          digitalWrite(LED_L,HIGH);
          endL = true;
          timerL.stopTimming();
          break;
        case 'D':
          digitalWrite(LED_R,LOW);
          digitalWrite(LED_L,LOW);
          digitalWrite(LED_EMPTY,HIGH);
          break;
        case 'E':
          error = 1;
          LcdMenuWrite("ERROR 1","TERCE OFFLINE",pushed,2,1000);
          submenu = 1;
          pushed = 1;
          break;
        case 'F':
          error = 2;
          break;
      }
    }
    prevTime = actTime; 
  }

  if((last_counter > counter) || (last_counter < counter)  || pushed) // LCD MENU se zobrazi pouze při změně stavu encoderu nebo stisknuti tlacitka
  {
    if(!started)
      button = false;
    //SUBMENU - VOLBA SPORTU - VIZUALIZACE
    if(submenu == 0)
    {  
      switch (counter)
      {
      case 0:
        LcdMenuWrite("Vyber sport","Pozarni sport",pushed,0);
        page=1;
        break;
      case 1:
        LcdMenuWrite("Vyber sport","5+1",pushed,0);
        page=2; 
        break;
      case 2:
        LcdMenuWrite("Vyber sport","Odpocet",pushed,0);
        page=3; 
        break;
      case 3:
        LcdMenuWrite("Vyber sport","Pocitadlo skore",pushed,0);
        page=4;
        break;
      }
    }//submenu = 0;

    //SUBMENU - POZARNI SPORT - VIZUALIZACE
    if(submenu == 1)
    {  
      switch(counter)
      {
        case 0:
          page=1;
          pushed=0;  
          LcdMenuWrite("Povolit start"," Start",pushed,1);
          break;
        case 1:
          page=2;
          pushed=0;  
          LcdMenuWrite("Start"," Reset",pushed,1);
          break;
        case 2:
          page=3;
          pushed=0;  
          LcdMenuWrite("Reset"," Stop",pushed,1);
          break;
        case 3:
          page=4;
          pushed=0;  
          LcdMenuWrite("Stop"," Vypustit terce",pushed,1);  
          break;
        case 4:
          page=5;
          pushed=0;
          LcdMenuWrite(" Stop","Vypustit terce",pushed,0);  
          break;      
      }
      Serial.print("page ");
      Serial.print(page);
      Serial.print("  counter");
      Serial.println(counter);
    }//submenu = 1;

    //SUBMENU Technicky vycvik druzstva 5+1 - VIZUALIZACE
    if(submenu == 2)
    {  
      switch (counter)
      {
        case 0:
          page=1;
          pushed=0;    
          LcdMenuWrite("Povolit start"," Start",pushed,1);
          break;
        case 1:   
          page=2;
          pushed=0;
          LcdMenuWrite("Start"," Stop",pushed,1);      
          break;
        case 2:
          page=3;
          pushed=0;      
          LcdMenuWrite("Stop"," Reset",pushed,1);
          break;
        case 3:
          page=4;
          pushed=0;  
          LcdMenuWrite(" Stop"," Reset",pushed,0);      
          break;
      }
    }//submenu = 2;

    //SUBMENU Odpocet - VIZUALIZACE
    if(submenu == 3)
    {  
      switch(counter)
      {
        case 0:
          page=1;
          pushed=0;      
          LcdMenuWrite("Start"," Stop",pushed,1);
          break;
        case 1:
          page=2;
          pushed=0;      
          LcdMenuWrite("Stop"," Reset",pushed,1);
          break;
        case 2:
          page=3;
          pushed=0;
          LcdMenuWrite("Reset"," Nastaveni casu",pushed,1);
          break;
        case 3:
          page=4;
          pushed=0; 
          LcdMenuWrite(" Reset","Nastaveni casu",pushed,0);   
          break;
      }
     }//submenu = 3;

    //SUBMENU Pocitadlo skore - VIZUALIZACE
    if(submenu == 4)
    {  
      switch(counter)
      {
        case 0:
          page=1;
          pushed=0;    
          LcdMenuWrite("Vynulovat"," Odecist bod D",pushed,1);
          break;
        case 1:
          page=2;
          pushed=0;      
          LcdMenuWrite("Odecist bod D"," Odecist bod H",pushed,1);
          break;
        case 2:
          page=3;
          pushed=0; 
          LcdMenuWrite(" Odecist bod D","Odecist bod H",pushed,0);     
          break;
      } 
    }//submenu = 4;
    Ready=1;
  }

  last_counter = counter; //ulozeni posledni hodnoty citace

    if(!digitalRead(ENC_BTN)) // akce po potvrzeni nabidky v menu
  {
    //SUBMENU - POZARNI SPORT - PRIKAZY
    if(submenu == 1)
    {  
      Serial.println(page);
      switch(page)
      {
        case 1:
          ClearMenuData();
          Serial.write("POVOLIT\n");
          timerReady = true;
          //mySerial.write("POVOLIT\n");
          break;
        case 2:
          ClearMenuData();
          //mySerial.write("START\n");
          if(timerReady)
          {
            Serial.write("START\n");
            started = true;
            timerL.init();
            timerR.init();
            timerL.startTimming();
            timerR.startTimming();
            timerReady = false;
          }
          break;
        case 3:
          ClearMenuData();
          //mySerial.write("RESET\n");
          Serial.write("RESET\n");
          delay(500);
          break;
        case 4:
          ClearMenuData();
          //mySerial.write("STOP\n");
          Serial.write("STOP\n");
          timerL.stopTimming();
          timerR.stopTimming();
          started = false;
          delay(500);
          break;
        case 5:
          ClearMenuData();
          //mySerial.write("STOP\n");
          Serial.write("VYPUSTIT\n");
          delay(500);
          break;
      }  
    }//end of submenu 1
    //SUBMENU Technicky vycvik druzstva 5+1 - PRIKAZY
    if(submenu == 2)
    { 
      switch(page)
      {
        case 1:
          ClearMenuData(); 
          delay(500);
          Serial.write("VYPUSTIT\n");
          break;
        case 2:
          ClearMenuData();
          Serial.write("STAV\n");
          delay(500);
          break;
        case 3:
          ClearMenuData();
          delay(500);
          break;
        case 4:
          ClearMenuData();
          delay(500);
          break;
        case 5:
          ClearMenuData();
          delay(500);
          break;
        case 6:
          ClearMenuData();
          delay(500);
          break;
      }   
    }//end of submenu 2
    //SUBMENU Odpocet - PRIKAZY
    if(submenu == 3)
    { 
      switch(page)
      {
        case 1:
          ClearMenuData();
          delay(500);
          break;
        case 2:
          ClearMenuData();
          delay(500);
          break;
        case 3:
          ClearMenuData();
          delay(500);
          break;
        case 4:
          ClearMenuData();
          delay(500);
          break;
      }      
    }//end of submenu 3
    //SUBMENU Pocitadlo skore - PRIKAZY
    if(submenu == 4)
    {    
      switch(page)
      {
        case 1:
          ClearMenuData();
          delay(500);
          //vynulovat_score();
          break;
        case 2:
          ClearMenuData();
          delay(500);
          //Domaci_minus();
          break;
        case 3:
          ClearMenuData();
          delay(500);
          //Hoste_minus(); 
          break;
      }
    }//end of submenu 4
    //SUBMENU - VOLBA SPORTU - PRIKAZY
  }

  if(submenu == 0 && !digitalRead(ENC_BTN))
    { 
      switch(page)
      {
        case 1:
          submenu=1;
          counter=0;
          pushed=1;
          delay(500);
          //Serial.println("SUBMENU1 & READY");
          break;
        case 2:
          submenu=2;
          counter=0;
          pushed=1;
          delay(500);
          break;
        case 3:
          submenu=3;
          counter=0;
          pushed=1;
          delay(500);
          break;
        case 4:
          submenu=4;
          counter=0;
          pushed=1;
          delay(500);
          break;
      }//end of submenu 0
    }


  aState = digitalRead(OutputA); // Reads the "current" state of the OutputA

  if (aState != aLastState)
  {     
    if (digitalRead(OutputB) != aState)
    {
      if(counter <6)
      counter ++;
    }
    else
    {
      if (counter > 0) 
        counter --;
      else counter = 0;
    } 

    //Serial.println(counter);  
  } 
    aLastState = aState;

}

void LcdMenuWrite (String text1,String text2,bool buttonPushed,int position,unsigned long int waitMs)
{
  lcd.clear();
  lcd.setCursor(0,0);
  if(position == 1)    /// ARROW UP
    lcd.write(1);
  lcd.print(text1);
  lcd.setCursor(0,1);  
  if(position == 0)   /// ARROW DOWN
    lcd.write(1);  
  lcd.print(text2);
  //page=1;
  if(buttonPushed)
    buttonPushed=0;
  delay(waitMs);  
}

void ClearMenuData()
{
  submenu=0;
  counter=0;
  pushed=0;
  Ready=0;
}

bool SendMessageSerial(String Msg)
{
  return true;
}

void PrintTime(TimerData data, bool id)
{
  if(id)
    lcd.setCursor(0,0);   //první řádek
  else
    lcd.setCursor(0,1);   //druhý řádek
  lcd.print("1. ");
  lcd.print(data.casTERC_M);
  lcd.print(":");
  lcd.print(data.casTERC_S);
  lcd.print(":");
  lcd.print(data.casTERC_ms);
  lcd.print("    ");
}