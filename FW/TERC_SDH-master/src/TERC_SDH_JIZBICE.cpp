/*************************************************************************
 *          Jednotka pro automaticke terce SDH CASOMIRY                  *
 *                                                                       *
 *      Autor: JAN DRSKA                                                 *
 *                                                                       *
 *      Posledni uprava: 21/03/2022                                      *
 *                                                                       *
 *************************************************************************/

#include <RF24.h>
#include <SPI.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>

RF24 radio(9, 10);  // CE, CSN

const byte addresses [] [6] = {"00001","00002"} ; // adresa komunikatoru
const char text1[] = "LEVY";
const char text2[] = "PRAVY";
const char text3[] = "PRAZDNO";
const char text4[] = "PRIPRAVEN";
#define terc1_1 2
#define terc1_2 3
#define terc2_1 4
#define terc2_2 5
#define majak1  6
#define majak2  7
#define ventil 8

bool tercL, tercP = LOW;
unsigned long cas = 0;
long predchozi_cas = 0;
char text [32] = "";
bool deliveryL_confirm_bit = LOW;     // boolovsky indikator pokud radio.write() tx byl uspesny
bool deliveryR_confirm_bit = LOW;     // boolovsky indikator pokud radio.write() tx byl uspesny
bool init_delivery_confirm_bit = LOW; // boolovsky indikator pokud radio.write() tx byl uspesny
bool reset = LOW;
bool init_var = HIGH;
String ZPRAVA;

void terceInit();

void setup()
{
  Serial.begin(9600);
  Serial.print("\nTERC_V1");  
  
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  //radio.setDataRate(RF24_250KBPS);
  radio.setRetries(4, 10);
  radio.openWritingPipe(addresses[1]); // 0002
  radio.openReadingPipe(1, addresses[0]); // 0001
  
  radio.startListening();  //nastaveni modulu jako vysilac
  terceInit();
  init_var = HIGH;

}
void loop()
{
  cas = millis();
  
  if(cas - predchozi_cas > 100) // kazdych 100 ms kontrola prijatych zprav - RESET,INIT apod. 
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

  if ((tercL && tercP)|| reset)
  {
   digitalWrite(ventil,HIGH);
   tercP = HIGH;
   tercL = HIGH;
  }
  //else digitalWrite(ventil,LOW);
  
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

// void terceInit - pocatecni inicializace tercu, test ventilu a svetelneho signalizacniho zarizeni
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

