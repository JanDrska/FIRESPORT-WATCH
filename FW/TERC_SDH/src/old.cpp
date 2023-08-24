/*************************************************************************
 *          Jednotka pro automaticke terce SDH CASOMIRY                  *
 *                                                                       *
 *      Autor: JAN DRSKA                                                 *
 *                                                                       *
 *      Posledni uprava: 02/04/2022                                      *
 *                                                                       *
 *************************************************************************/
/*
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
String ZPRAVA;
char text [32] = "";
const char text1[] = "LEVY";
const char text2[] = "PRAVY";
const char text3[] = "PRAZDNO";
const char text4[] = "PRIPRAVEN";

bool target1, target2 = LOW;  // stavy tercu
unsigned long time = 0;
long previousTime = 0;
bool reset = LOW;

void terceInit();

void target1ISR() // preruseni po naplneni - interrupt na zmenu
{
  target1 = true;
}

void target2ISR() // preruseni po naplneni - interrupt na zmenu
{
  target2 = true;
}


void setup()
{

//#### NASTAVENI PINU a PRERUSENI NA PINECH
  pinMode(signalLight1,OUTPUT);
  pinMode(signalLight2,OUTPUT);
  pinMode(waterValve,OUTPUT);

  pinMode(target1Full,INPUT);
  attachInterrupt(digitalPinToInterrupt(target1Full),target1ISR,CHANGE);
  pinMode(target1Empty,INPUT);
  pinMode(target2Full,INPUT);
  attachInterrupt(digitalPinToInterrupt(target2Full),target2ISR,CHANGE);
  pinMode(target2Empty,INPUT);

//#### NASTAVENI A ZAHAJENI SERIOVE KOMUNIKACE ####
  Serial.begin(9600);
  Serial.print("\nTERC_V1");  

//#### NASTAVENI A ZAHAJENI RADIOVE KOMUNIKACE ####
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  //radio.setDataRate(RF24_250KBPS);
  radio.setRetries(4, 10);
  radio.openWritingPipe(addresses[1]); // 0002
  radio.openReadingPipe(1, addresses[0]); // 0001
  
  radio.startListening();  //nastaveni modulu jako vysilac

// #### PRVOTNI INICIALIZACE TERCU ####
  terceInit();
}

void loop()
{
  time = millis();
  
// #### PERIODICKA KONTROLA PRIJATYCH ZPRAV ####

  if(time - previousTime > 50) // kazdych 50 ms kontrola prijatych zprav - RESET,INIT apod. 
  {
    radio.startListening();
    if(radio.available())
    {
      radio.read(&text, sizeof(text));
      String ZPRAVA = String(text);
      Serial.println(ZPRAVA);
      if (ZPRAVA == "INIT") terceInit();
      if (ZPRAVA == "RESET") reset = HIGH;
    }
    previousTime = time;   
  }
  

  //digitalRead(target1Full)



// #### DETEKCE NAPLNENI TERCE CISLO 1 ####

  if ((digitalRead(target1Full) == 1) && !target1)
  {
      //Serial.println(digitalRead(target1Full));
    radio.stopListening();
    if(radio.write(&text1, sizeof(text1)))
    {
      Serial.write("\nLEVY");
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
      Serial.write("\nPRAVY");
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
    Serial.write("\nPRAZDNO");
    digitalWrite(waterValve,LOW);
    digitalWrite(signalLight1,LOW);
    digitalWrite(signalLight2,LOW);
    target1 = target2 = LOW;
  } 
}

// #### POCATECNI INICIALIZACE TERCU , TEST waterValveU a SVETELNE SIGNALIZACE ####

void terceInit()    
{
  Serial.write("\nINIT");
  Serial.write("\nSV1");
  digitalWrite(signalLight1,HIGH);
  digitalWrite(signalLight2,HIGH);
  digitalWrite(waterValve,HIGH);
  delay(2000);
  radio.stopListening();
  if(!radio.write(&text4, sizeof(text4))) // pokud se zprava neodesle, zacne blikat majak
    terceInit();
  radio.startListening();
  Serial.write("\nSV1 LOW");
  digitalWrite(signalLight1,LOW);
  digitalWrite(signalLight2,LOW);
  digitalWrite(waterValve,LOW);
}

*/