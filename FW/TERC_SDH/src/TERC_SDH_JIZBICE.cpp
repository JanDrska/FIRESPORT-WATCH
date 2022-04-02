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
#define terc1_1 2   // TERC 1 PLNY
#define terc1_2 3   // TERC 1 PRAZDNY
#define terc2_1 4   // TERC 2 PLNY
#define terc2_2 5   // TERC 2 PRAZDNY
#define majak1  6   // MAJAK TERC 1
#define majak2  7   // MAJAK TERC 2
#define ventil 8    // VYSTUP PRO OVLADANI VENTILU (ON/OFF)

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

void setup()
{

//#### NASTAVENI A ZAHAJENI SERIOVE KOMUNIKACE ####
  Serial.begin(9600);
  Serial.print("\nTERC_V1");  

//#### NASTAVENI A ZAHAJENI RADIOVE KOMUNIKACE ####
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
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

// #### DETEKCE NAPLNENI TERCE CISLO 1 ####

  if ((digitalRead(terc1_1) == HIGH) && !tercL)
  {
    radio.stopListening();
    if(radio.write(&text1, sizeof(text1)))
    {
      Serial.write("\nLEVY");
      tercL = HIGH;
      digitalWrite(majak1,HIGH);
    }
    radio.startListening(); 
  } 

  // #### DETEKCE NAPLNENI TERCE CISLO 2 ####

  if ((digitalRead(terc2_1) == HIGH) && !tercP)
  {
    radio.stopListening();
    if(radio.write(&text2, sizeof(text2)))
    {
      Serial.write("\nPRAVY");
      tercP = HIGH;
      digitalWrite(majak2,HIGH);
    }
    radio.startListening();
  } 

  // #### VYPOUSTENI ####

  if ((tercL && tercP)|| reset)
  {
   digitalWrite(ventil,HIGH);
   tercP = HIGH;
   tercL = HIGH;
  }
  //else digitalWrite(ventil,LOW);

  // #### DETEKCE PRAZDNYCH TERCU ####
  
  if ((digitalRead(terc1_2) && digitalRead(terc2_2)) && (tercL && tercP))
  {
    reset = LOW;
    radio.stopListening();
    radio.write(&text3, sizeof(text3));
    radio.startListening();
    Serial.write("\nPRAZDNO");
    digitalWrite(ventil,LOW);
    digitalWrite(majak1,LOW);
    digitalWrite(majak2,LOW);
    tercL = tercP = LOW;
  } 
}

// #### POCATECNI INICIALIZACE TERCU , TEST VENTILU a SVETELNE SIGNALIZACE ####

void terceInit()    
{
  Serial.write("\nINIT");
  digitalWrite(majak1,HIGH);
  digitalWrite(majak2,HIGH);
  digitalWrite(ventil,HIGH);
  delay(2000);
  radio.stopListening();
  if(!radio.write(&text4, sizeof(text4))) terceInit();
  radio.startListening();
  digitalWrite(majak1,LOW);
  digitalWrite(majak2,LOW);
  digitalWrite(ventil,LOW);
}

