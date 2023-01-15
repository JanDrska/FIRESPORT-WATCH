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

RF24 radio(9, 10);  // CE, CSN
#define target1Full 8   // TERC 1 PLNY
#define target1Empty 7   // TERC 1 PRAZDNY
#define target2Full 6   // TERC 2 PLNY
#define target2Empty 5  // TERC 2 PRAZDNY
#define signalLight1  4   // MAJAK TERC 1
#define signalLight2  3   // MAJAK TERC 2
#define waterValve 2    // VYSTUP PRO OVLADANI waterValveU (ON/OFF)

//#### GLOBALNI PROMENNE ####

const byte addresses [] [6] = {"00001","00002"} ; // adresa komunikatoru
String ZPRAVA;
char text [32] = "";
const char text1[] = "LEVY";
const char text2[] = "PRAVY";
const char text3[] = "PRAZDNO";
const char text4[] = "PRIPRAVEN";

bool tercL, tercP = LOW;  // stavy tercu
unsigned long cas = 0;
long predchozi_cas = 0;
bool reset = LOW;

void terceInit();
void terceInitFail();

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
  cas = millis();
  
// #### PERIODICKA KONTROLA PRIJATYCH ZPRAV ####

  if(cas - predchozi_cas > 50) // kazdych 50 ms kontrola prijatych zprav - RESET,INIT apod. 
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
    predchozi_cas = cas;   
  }

  //digitalRead(target1Full)



// #### DETEKCE NAPLNENI TERCE CISLO 1 ####

  if ((digitalRead(target1Full) == 1) && !tercL)
  {
      //Serial.println(digitalRead(target1Full));
    radio.stopListening();
    if(radio.write(&text1, sizeof(text1)))
    {
      Serial.write("\nLEVY");
      tercL = HIGH;
      digitalWrite(signalLight1,HIGH);
    }
    radio.startListening(); 
  } 

  // #### DETEKCE NAPLNENI TERCE CISLO 2 ####

  if ((digitalRead(target2Full) == 1) && !tercP)
  {
    radio.stopListening();
    if(radio.write(&text2, sizeof(text2)))
    {
      Serial.write("\nPRAVY");
      tercP = HIGH;
      digitalWrite(signalLight2,HIGH);
    }
    radio.startListening();
  } 

  // #### VYPOUSTENI ####

  if ((tercL && tercP)|| reset)
  {
   digitalWrite(waterValve,HIGH);
   tercP = HIGH;
   tercL = HIGH;
  }
  //else digitalWrite(waterValve,LOW);

  // #### DETEKCE PRAZDNYCH TERCU ####
  
  if ((digitalRead(target1Empty) && digitalRead(target2Empty)) && (tercL && tercP))
  {
    reset = LOW;
    radio.stopListening();
    radio.write(&text3, sizeof(text3));
    radio.startListening();
    Serial.write("\nPRAZDNO");
    digitalWrite(waterValve,LOW);
    digitalWrite(signalLight1,LOW);
    digitalWrite(signalLight2,LOW);
    tercL = tercP = LOW;
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

void terceInitFail()
{

}

