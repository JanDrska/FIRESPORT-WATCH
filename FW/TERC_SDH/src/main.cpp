/*************************************************************************
 *          Jednotka pro automaticke terce SDH CASOMIRY                  *
 *                                                                       *
 *      Autor: JAN DRSKA                                                 *
 *                                                                       *
 *      Posledni uprava: 02/04/2022                                      *
 *                                                                       *
 *************************************************************************/

#include <RF24.h>
#include <SPI.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>


//#### HW KONFIGURACE ####

RF24 radio(9, 10);      // CE, CSN
#define target1Full 8   // TERC 1 PLNY
#define target1Empty 7  // TERC 1 PRAZDNY
#define target2Full 6   // TERC 2 PLNY
#define target2Empty 5  // TERC 2 PRAZDNY
#define signalLight1  4 // MAJAK TERC 1
#define signalLight2  3 // MAJAK TERC 2
#define waterValve 2    // VYSTUP PRO OVLADANI waterValveU (ON/OFF)

//#### GLOBALNI PROMENNE ####

const byte addresses [] [6] = {"00001","00002"} ; // adresa komunikatoru
String message;
char text [32] = "";
const char text1[] = "target1";
const char text2[] = "target2";
const char text3[] = "empty";
const char text4[] = "ready";
const char text5[] = "init";

bool target1, target2 = LOW;  // stavy tercu
unsigned long time = 0;
long prevTime = 0;
bool reset = LOW;

void initTargets();

void setup()
{

  pinMode(signalLight1,OUTPUT);
  pinMode(signalLight2,OUTPUT);
  pinMode(waterValve,OUTPUT);
  
  pinMode(target1Full,INPUT);
  pinMode(target1Empty,INPUT);
  pinMode(target2Full,INPUT);
  pinMode(target2Empty,INPUT);

//#### NASTAVENI A ZAHAJENI SERIOVE KOMUNIKACE ####
  Serial.begin(9600);
  Serial.print("\nTerc V2.0 by Jan Drska");  

//#### NASTAVENI A ZAHAJENI RADIOVE KOMUNIKACE ####
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  //radio.setDataRate(RF24_250KBPS);
  radio.setRetries(4, 10);
  radio.openWritingPipe(addresses[1]); // 0002
  radio.openReadingPipe(1, addresses[0]); // 0001
  
  radio.startListening();  //nastaveni modulu jako vysilac

// #### PRVOTNI INICIALIZACE TERCU ####
  initTargets();
}

void loop()
{
  time = millis();
  
// #### PERIODICKA KONTROLA PRIJATYCH ZPRAV ####

  if(time - prevTime > 50) // kazdych 50 ms kontrola prijatych zprav - RESET,INIT apod. 
  {
    radio.startListening();
    if(radio.available())
    {
      radio.read(&text, sizeof(text));
      String message = String(text);
      Serial.println(message);
      if (message == "INIT") initTargets();
      if (message == "RESET") reset = HIGH;
    }
    prevTime = time;   
  }

  //digitalRead(target1Full)



// #### DETEKCE NAPLNENI TERCE CISLO 1 ####

  if ((digitalRead(target1Full) == 1) && !target1)
  {
      //Serial.println(digitalRead(target1Full));
    radio.stopListening();
    if(radio.write(&text1, sizeof(text1)))
    {
      Serial.write("\ntarget1");
      target1 = HIGH;
      digitalWrite(signalLight1,HIGH);
    }
    radio.startListening(); 
  } 

  // #### DETEKCE NAPLNENI TERCE CISLO 2 ####

  if ((digitalRead(target2Full) == 1) && !target2)
  {
    radio.stopListening();
    if(radio.write(&text2, sizeof(text2)))
    {
      Serial.write("\ntarget2");
      target2 = HIGH;
      digitalWrite(signalLight2,HIGH);
    }
    radio.startListening();
  } 

  // #### VYPOUSTENI ####

  if ((target1 && target2)|| reset)
  {
   digitalWrite(waterValve,HIGH);
   target2 = HIGH;
   target1 = HIGH;
  }
  //else digitalWrite(waterValve,LOW);

  // #### DETEKCE PRAZDNYCH TERCU ####
  
  if ((digitalRead(target1Empty) && digitalRead(target2Empty)) && (target1 && target2))
  {
    reset = LOW;
    radio.stopListening();
    radio.write(&text3, sizeof(text3));
    radio.startListening();
    Serial.write("\nempty");
    digitalWrite(waterValve,LOW);
    digitalWrite(signalLight1,LOW);
    digitalWrite(signalLight2,LOW);
    target1 = target2 = LOW;
  } 
}

// #### POCATECNI INICIALIZACE TERCU , TEST waterValveU a SVETELNE SIGNALIZACE ####

void initTargets()    
{
  bool initDone = false;
  bool blinkState = false;
  size_t initMode = 0;
  unsigned long time = 0, prevBlinkTime = 0;

  prevBlinkTime = millis();

  while(!initDone)
 {
    time = millis();
    if((time-prevBlinkTime)>1000)
    {
      digitalWrite(signalLight1,!blinkState);
      digitalWrite(signalLight2,!blinkState);
      prevBlinkTime = millis();
    }

    switch (initMode)
    {
      case 0:
        Serial.write("\nINIT");
        digitalWrite(signalLight1,HIGH);
        digitalWrite(signalLight2,HIGH);
        digitalWrite(waterValve,HIGH);
        radio.stopListening();
        initMode = 1;
      break;
      case 1:
        if(radio.write(&text5, sizeof(text5))) // pokud se zprava neodesle opakujeme vysilani
          initMode = 2;
      break;

      case 2:
        radio.stopListening();
        if(radio.available())
          radio.read(&text,sizeof(text));
        if(text == text5)                      // Pokud prijde echo, pokracujeme dal
          initMode = 3;
      break;

      case 3:
        digitalWrite(signalLight1,LOW);
        digitalWrite(signalLight2,LOW);
        digitalWrite(waterValve,LOW);
        initDone = true;
        initMode = 0;
      break;
    }
  }
}

