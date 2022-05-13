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
/*
#include <Wire.h> 
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);  

int counter = 0; 
int page=1;
int Ready=0;
int submenu=0;
int pushed = 0;
int aState;
int aLastState; 
int last_counter=0; 

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

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(1);  
  lcd.print("  SDH JIZBICE");
  lcd.setCursor(0,1);  
  lcd.print("       V1");

  delay(1000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Vyber sport");
  lcd.setCursor(0,1);  
  lcd.write(1);  
  lcd.print("Pozarni sport");

  //mySerial.begin(9600);

}

void loop()
{ 
  if((last_counter > counter) || (last_counter < counter)  || pushed) // LCD MENU se zobrazi pouze při změně stavu encoderu nebo stisknuti tlacitka
  {
    Ready=1;
    
    //SUBMENU - VOLBA SPORTU
    if(submenu == 0)
    {  
      if(counter == 0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Vyber sport");
        lcd.setCursor(0,1);  
        lcd.write(1);  
        lcd.print("Pozarni sport");
        page=1;
        if(pushed)
        {
          pushed=0;
        } 
      }
    
      if(counter == 1)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Vyber sport");
        lcd.setCursor(0,1);
        lcd.write(1);   
        lcd.print("5+1");
        page=2; 
        if(pushed)
        {
          pushed=0;
        }   
      }
      
      if(counter ==2)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Vyber sport");
        lcd.setCursor(0,1);  
        lcd.write(1);  
        lcd.print("Odpocet");
        page=3; 
        if(pushed)
        {
          pushed=0;
        }   
      }
    
      if(counter ==3 )
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Vyber sport");
        lcd.setCursor(0,1);
        lcd.write(1);   
        lcd.print("Pocitadlo skore");
        page=4;
        if(pushed)
        {
          pushed=0;
        }  
      }
    }//submenu = 0;


    //SUBMENU - POZARNI SPORT
    if(submenu == 1)
      {  
        if(counter == 0)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1);  
          lcd.print("Povolit start");
          lcd.setCursor(0,1);  
          lcd.print(" Start");
          page=1;
          pushed=0;   
          //Serial.println("SUBMENU 1,1"); 
        }
      
        if(counter == 1)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1);   
          lcd.print("Start");
          lcd.setCursor(0,1);
          lcd.print(" Reset");
          page=2;
          pushed=0;  
          //Serial.println("SUBMENU 1,2");    
        }
      
        if(counter == 2)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1);  
          lcd.print("Reset");
          lcd.setCursor(0,1);  
          lcd.print(" Stop");
          page=3;
          pushed=0;  
          //Serial.println("SUBMENU 1,3");    
        }
      
        if(counter == 3)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1);   
          lcd.print("Stop");
          lcd.setCursor(0,1);
          lcd.print(" Vypustit terce");
          page=4;
          pushed=0;    
          //Serial.println("SUBMENU 1,4");    
        }
        if(counter == 4)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1);   
          lcd.print("Vypustit terce");
          lcd.setCursor(0,1);
          lcd.print(" Stav tercu");
          page=5;
          pushed=0;        
          //Serial.println("SUBMENU 1,5");
        }
        if(counter == 5)
        {
          lcd.clear();
          lcd.setCursor(0,0);   
          lcd.print(" Vypustit terce");
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.print("Stav tercu");
          page=6;
          pushed=0;     
          //Serial.println("SUBMENU 1,6");   
        }
      }//submenu = 1;

    //SUBMENU Technicky vycvik druzstva 5+1
    if(submenu == 2)
    {  
      if(counter == 0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);  
        lcd.print("Povolit start");
        lcd.setCursor(0,1);  
        lcd.print(" Start");
        page=1;
        pushed=0;    
      }
    
      if(counter == 1)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);   
        lcd.print("Start");
        lcd.setCursor(0,1);
        lcd.print(" Stop");
        page=2;
        pushed=0;      
      }
    
      if(counter == 2)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);  
        lcd.print("Stop");
        lcd.setCursor(0,1);  
        lcd.print(" Reset");
        page=3;
        pushed=0;      
      }
    
      if(counter == 3)
      {
        lcd.clear();
        lcd.setCursor(0,0);  
        lcd.print(" Stop");
        lcd.setCursor(0,1); 
        lcd.write(1); 
        lcd.print("Reset");
        page=4;
        pushed=0;        
      }
    }//submenu = 2;


    //SUBMENU Odpocet
    if(submenu == 3)
    {  
      
      if(counter == 0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);   
        lcd.print("Start");
        lcd.setCursor(0,1);
        lcd.print(" Stop");
        page=1;
        pushed=0;      
      }
    
      if(counter == 1)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);  
        lcd.print("Stop");
        lcd.setCursor(0,1);  
        lcd.print(" Reset");
        page=2;
        pushed=0;      
      }
    
      if(counter == 2)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);   
        lcd.print("Reset");
        lcd.setCursor(0,1); 
        lcd.print(" Nastaveni casu");
        page=3;
        pushed=0;
      }  

       if(counter == 3)
       {
         lcd.clear();
        lcd.setCursor(0,0);   
        lcd.print("Reset");
        lcd.setCursor(0,1); 
        lcd.write(1);
        lcd.print(" Nastaveni casu");
         page=4;
         pushed=0;    
       }      
     }//submenu = 3;

  
    if(submenu == 4)
    {  
      if(counter == 0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);  
        lcd.print("Vynulovat");
        lcd.setCursor(0,1);  
        lcd.print(" Odecist bod D");
        page=1;
        pushed=0;    
      }
    
      if(counter == 1)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);   
        lcd.print("Odecist bod D");
        lcd.setCursor(0,1);
        lcd.print(" Odecist bod H");
        page=2;
        pushed=0;      
      }
    
      if(counter == 2)
      {
        lcd.clear();
        lcd.setCursor(0,0);   
        lcd.print(" Odecist bod D");
        lcd.setCursor(0,1);
        lcd.write(1);
        lcd.print("Odecist bod H");
        page=3;
        pushed=0;      
      }
    }//submenu = 4;
  }

  last_counter = counter; //ulozeni posledni hodnoty citace

  if(!digitalRead(ENC_BTN))
  {
    if(submenu == 1)
    {    
      if(page==1)
      {
      submenu=0;
      counter=1;
      pushed=0;
      Ready=0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PROVADIM");
      lcd.setCursor(0,1);  
      lcd.print("PROSIM CEKEJTE");
      //lcd.clear();
      //pozarni_sport();
      //Serial.println("SUBMENU 1 str1");
      //mySerial.write("POVOLIT\n");
      Serial.write("POVOLIT\n");
      delay(4000);
      }
    
      if(page==2)
      {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        //lcd.clear();
        //5+1();
        //Serial.println("SUBMENU 1 str2");
        //mySerial.write("START\n");
        Serial.write("START\n");
       }
    
      if(page==3)
      {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        //lcd.clear();
        //odpocet();
        //Serial.println("SUBMENU 1 str3");
        //mySerial.write("RESET\n");
        Serial.write("RESET\n");
        delay(500);
      }
    
       if(page==4)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        //lcd.clear();
        //skore();
        //Serial.println("SUBMENU 1 str4");
        //mySerial.write("STOP\n");
        Serial.write("STOP\n");
        delay(500);
       }
    }//end of submenu 1

    if(submenu == 2)
    {    
       if(page==1)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0; 
        delay(500);
        Serial.write("VYPUSTIT\n");
        //lcd.clear();
        //povolit_start();
       }
    
       if(page==2)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        Serial.write("STAV\n");
        delay(500);
        //lcd.clear();
        //start();
       }
    
       if(page==3)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //reset();
       }
    
       if(page==4)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //stop();
       }

       if(page==5)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //vypustit_terce();
       }

       if(page==6)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //stav_tercu();
       }
    }//end of submenu 2

    if(submenu == 3)
    {    
       if(page==1)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //start_odpoctu();
       }
    
       if(page==2)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //stop();
       }  

       if(page==2)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //reset();
       }  

       if(page==2)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //nastaveni_casu();
       }    
    }//end of submenu 3

    if(submenu == 4)
    {    
       if(page==1)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //vynulovat_score();  
       }
    
       if(page==2)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //Domaci_minus();
       }
    
       if(page==3)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //Hoste_minus();  
       }
    }//end of submenu 4
  

  if(submenu == 0 && Ready==1)
    {    
       if(page==1)
       {
        submenu=1;
        counter=0;
        pushed=1;
        delay(500);
        Serial.println("SUBMENU1 & READY");
       }
    
       if(page==2)
       {
        submenu=2;
        counter=0;
        pushed=1;
        delay(500);
       }
    
       if(page==3)
       {
        submenu=3;
        counter=0;
        pushed=1;
        delay(500);
       }
    
       if(page==4)
       {
        submenu=4;
        counter=0;
        pushed=1;
        delay(500);
       }
    }//end of submenu 0
  }

  aState = digitalRead(OutputA); // Reads the "current" state of the OutputA

   if (aState != aLastState)
   {     
     if (digitalRead(OutputB) != aState)
     {
      if(counter <6)counter ++;
     }
     else
     {
      if (counter > 0) counter --;
      else counter = 0;
     }
     
     Serial.print("Position: ");
     Serial.println(counter);
   } 

  aLastState = aState;

}
*/