/*************************************************************************
 *          Jednotka pro automaticke terce SDH CASOMIRY                  *
 *                                                                       *
 *      Autor: JAN DRSKA                                                 *
 *                                                                       *
 *      Posledni uprava: 24/08/2023                                      *
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

uint8_t state = 0;
const byte addresses [] [6] = {"00001","00002"} ; // adresa komunikatoru
String ZPRAVA;
char text [32] = "";
const char text1[] = "TERC1";
const char text2[] = "TERC2";
const char text3[] = "EMPTY";
const char text4[] = "READY";
const char text5[] = "PING";
const char text6[] = "PONG";

const char cmd1[] = "ENABLE";
const char cmd2[] = "RESET";
const char cmd3[] = "EMPTY_OUT";

bool target1, target2 = LOW;  // stavy tercu
bool initSuccess = false;
bool emptyOut = false;
bool startEnabled = false;
bool dataTransfered= false;
unsigned long time = 0;
long previousTime = 0;
bool reset = LOW;

void target1ISR() // preruseni po naplneni - interrupt na nabeznou hranu
{
  target1 = true;
  digitalWrite(signalLight1,HIGH);
}

void target2ISR() // preruseni po naplneni - interrupt na nabeznou hranu
{
  target2 = true;
  digitalWrite(signalLight2,HIGH);
}

void setup()
{

//#### NASTAVENI PINU a PRERUSENI NA PINECH
  pinMode(signalLight1,OUTPUT);
  pinMode(signalLight2,OUTPUT);
  pinMode(waterValve,OUTPUT);

  pinMode(target1Full,INPUT);
  attachInterrupt(digitalPinToInterrupt(target1Full),target1ISR,RISING);
  pinMode(target1Empty,INPUT);
  pinMode(target2Full,INPUT);
  attachInterrupt(digitalPinToInterrupt(target2Full),target2ISR,RISING);
  pinMode(target2Empty,INPUT);

//#### NASTAVENI A ZAHAJENI SERIOVE KOMUNIKACE ####
  Serial.begin(9600);
  Serial.print("\nTERC_V1");  

//#### NASTAVENI A ZAHAJENI RADIOVE KOMUNIKACE ####
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  //radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true); // nastaveni automatickeho potvrzeni prijmu dat
  radio.setRetries(10, 10); // nastaveni 10 pokusu po 10 ms
  radio.openWritingPipe(addresses[1]); // 0002
  radio.openReadingPipe(1, addresses[0]); // 0001
  
  radio.startListening();  //nastaveni modulu jako prijimac
}

void loop()
{
  radio.startListening();
  if (radio.available()) 
  {
    radio.read(&text, sizeof(text)); // Přijetí dat

    if(strncmp(text,cmd1,sizeof(cmd1)))
      startEnabled =  true;
    else if (strncmp(text,cmd2,sizeof(cmd2)))
      reset = true;
    else if (strncmp(text,cmd3,sizeof(cmd3)))
      emptyOut = true;
    else
      Serial.write("Unknown command");

  }

  switch (state)
  {
    case 0: // INIT
      +-+
      digitalWrite(signalLight1,HIGH);
      digitalWrite(signalLight2,HIGH);
      //NAVAZANI SPOJENI S USTREDNOU, JINAK OPAKUJEME DOKUD NENI SPOJENI NAVAZANO
      while(!initSuccess)
      {
        if (radio.available()) 
          radio.read(&text, sizeof(text));
        if(text == text5)
        {
          radio.stopListening();
          if(radio.write(text6,sizeof(text6)))
            initSuccess = true;
          radio.startListening();
        }
      }
      delay(2000);
      digitalWrite(signalLight1,LOW);
      digitalWrite(signalLight2,LOW);
      delay(2000);
      state = 1;
      break;
    
    case 1: // CEKANI NA POVOLENI STARTU
      digitalWrite(signalLight1,HIGH);
      digitalWrite(signalLight2,HIGH);
      if(startEnabled)
      {
        radio.stopListening();
        if(radio.write(text4,sizeof(text4)))
        {
          state = 2;
          radio.startListening();
        }
        else
          state = 5;
        radio.startListening();
      }
      break;

    case 2: // PLNENI TERCU
      startEnabled = false;
      digitalWrite(signalLight1,LOW);
      digitalWrite(signalLight2,LOW);
      if(target1)
      {
        //digitalWrite(signalLight1,HIGH);
        // odeslani zpravy o naplneni terce 1
        radio.stopListening();
        if(radio.write(text1,sizeof(text1)))
          radio.startListening();
        else
          state = 5;
        radio.startListening();
      }
      if(target2)
      {
        //digitalWrite(signalLight1,HIGH);
        // odeslani zpravy o naplneni terce 2
        radio.stopListening();
        if(radio.write(text2,sizeof(text2)))
          radio.startListening();
        else
          state = 5;
        radio.startListening();
      }
      if(target1 && target2)
        state = 3;
      break;

    case 3: // VYPRAZDNOVANI TERCU
        digitalWrite(waterValve,HIGH);
        if(digitalRead(target1Empty) && digitalRead(target2Empty))
        {
          
          radio.stopListening();
          digitalWrite(waterValve,LOW);
          if(radio.write(text3,sizeof(text3)))
          {
            target1 = false;
            target2 = false;
            state = 1;
          }
          radio.startListening();
        }
      break;

    case 4: // TODO PRIJATY PRIKAZ

      break;

    case 5: // TODO ERROR ZTRATA KOMUNIKACE

      break;
  }

}
