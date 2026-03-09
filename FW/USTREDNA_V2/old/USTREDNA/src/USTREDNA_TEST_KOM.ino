#include <SPI.h>
#include <RF24.h>
#include <FastCRC.h> // Přidej knihovnu FastCRC

RF24 radio(9, 10); // Definice pinů pro modul NRF24L01+
const uint64_t pipe = 0xE8E8F0F0E1LL; // Adresa, na které budou komunikovat oba moduly
FastCRC8 CRC8; // Vytvoření instance pro výpočet CRC8

unsigned long currentTime, lastRequestTime;

void printBinary(uint8_t value) 
{
  Serial.print("0b");
  for (int i = 7; i >= 0; i--)
    Serial.print(bitRead(value, i));
  Serial.println();
}

void setup() 
{
  Serial.begin(115200);
  Serial.println("USTREDNA FW v3.0 TEST");
  while (!radio.begin()) 
  {
    Serial.println(F("radio hardware is not responding!!"));
    //while (1) {}  // nutno resetovat
  }
  radio.setPALevel(RF24_PA_HIGH);  // PAK DAT HIGH
  radio.openWritingPipe(pipe);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
}

void loop() 
{
  radio.startListening();
  currentTime = millis();
  
  // Odeslat žádost o stavy každých 100 ms
  if (currentTime - lastRequestTime >= 5) 
  {
    lastRequestTime = currentTime;

    // Žádost o stavy
    uint8_t dataToSend = 0xFF; // Tvoje data k odeslání
    uint8_t crcValue = CRC8.smbus((uint8_t*)&dataToSend, 15);

    // Vytvoř zprávu
    uint8_t message[2];
    message[0] = dataToSend;
    message[1] = crcValue;

    radio.stopListening();
    radio.write(message, sizeof(message));
    radio.startListening();

    // Čekání na odpověď
    unsigned long startTime = millis();
    while (!radio.available() && millis() - startTime < 1000) 
    {
      // Počkej na odpověď nebo 1 sekundu
      //Serial.println("ERROR - BEZ ODPOVEDI");
    }
  }
    if (radio.available()) 
    {
      uint8_t response[2];
      radio.read(&response, sizeof(response));

      // Zpracuj odpověď (data a CRC)
      uint8_t receivedData = response[0];
      uint8_t receivedCRC = response[1];

      Serial.print("Prijata data: ");
      Serial.print(receivedData);
      Serial.print(" Prijate CRC: ");
      Serial.println(receivedCRC);

      // Porovnej přijaté CRC s vypočítaným CRC
      uint8_t calculatedCRC = CRC8.smbus((uint8_t*)&receivedData, 1);
      if (receivedCRC == calculatedCRC) 
      {
        // CRC kontrola je v pořádku, můžeš zpracovat data
        // receivedData obsahuje přijatá data
        Serial.print("Prijata data: ");
        printBinary(receivedData);
      } 
      else 
      {
        Serial.println("CHYBA CRC");
        // Chyba CRC, zpráva může být poškozena
      }
    }

  // Zde můžeš provádět další úkoly
}
