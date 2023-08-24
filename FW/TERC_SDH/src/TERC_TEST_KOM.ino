#include <SPI.h>
#include <RF24.h>
#include <FastCRC.h> // Přidej knihovnu FastCRC

RF24 radio(9, 10); // Definice pinů pro modul NRF24L01+
const uint64_t pipe = 0xE8E8F0F0E1LL; // Adresa, na které budou komunikovat oba moduly
FastCRC8 CRC8; // Vytvoření instance pro výpočet CRC8

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
  if (!radio.begin()) 
  {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // nutno resetovat
  }
  radio.setPALevel(RF24_PA_HIGH); // pak dat high !!
  radio.openWritingPipe(pipe);
  radio.openReadingPipe(1, pipe);
  radio.startListening();
}

void loop() 
{
  if (radio.available()) 
  {
    uint8_t request[2];
    radio.read(&request, sizeof(request));

    // Zpracuj odpověď (data a CRC)
    uint8_t receivedData = request[0];
    uint8_t receivedCRC = request[1];

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

    // Pokud je žádost o stavy
    if (receivedData == 255) 
    {
      Serial.println("STATUS REQUESTED");
      uint8_t dataToSend = 42; // Tvoje data k odeslání
      uint8_t crcValue = CRC8.smbus((uint8_t*)&dataToSend, 1);

      // Vytvoř zprávu
      uint8_t message[2];
      message[0] = dataToSend;
      message[1] = crcValue;

      // Odeslat stavy ústředně
      radio.stopListening();
      Serial.print("Odesilam: ");
      printBinary(dataToSend);
      radio.write(message, sizeof(message));
      radio.startListening();
    }
  }

  // Zde můžeš provádět další úkoly
}
