//####### PROGRAM HASICSKE STOPKY  ######
// PROGRAM VYTVORIL JAN DRSKA PRO JEDNOTKU SDH JIZBICE
// PROGRAM JE VYHRADNIM VLASTNICTVIM AUTORA A SDH JIZBICE - JAKEKOLIV SIRENI PROGRAMU BEZ VEDOMI AUTORA JE ZAPOVEZENO!!!
/*
              /----/  /----/        /----/  /----/
             /    /  /    /  /--/  /    /  /    /
            /----/  /----/        /----/  /----/
           /    /  /    /  /--/  /    /  /    /
          /----/  /----/        /----/  /----/
*/

// VERZE 2.0
// POSLEDNI UPRAVA 28.09.2021

#include "sdhTimer.h"
#include "sdhDisplay.h"
#include <LiquidCrystal_I2C.h>
#include <PCF8574.h>
#include <SPI.h>
#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>

#define TLA 2      //tlačítko A - START
#define TLB 3      //tlačítko B - RESET
#define TLProg 8   //prepinac programu

bool terc1, terc2, prazdno = LOW;    //terc 1 - LEVY a 2 - PRAVY


#define ODPOCET_TERC 300000  //pozadovany cas v ms (300 000 je 5 minut)

TimerData dataL;
TimerData dataR;

DisplaySdh display;

typedef struct TERC_DATA 
{
  long casSTART = 0;          //startovní čas
  long casODPOCET = 0;
  long casTERC = 0;          //průběžný čas 1. terče
  long casTERC_end = 0;       //konečný čas 1. terče
  long casTERC_M = 0;         //konečný čas 1. terče - minuty
  long casTERC_S = 0;         //konečný čas 1. terče - sekundy
  long casTERC_ms = 0;        //konečný čas 1. terče - ms
};

struct TERC_DATA TERC_L;
struct TERC_DATA TERC_P;

bool init_delivery_confirm_bit = LOW;
bool READY = LOW;
const char text1[] = "INIT";
const char text2[] = "RESET";
const char text3[] = "IS_READY";
const char text4 [32] = "";
char text [32] = "";


int STOP1 = 0;
int STOP2 = 0;
int program =0;

// dp ABCGFED

byte cislice[10] = 
{B01110111,B00110000, // 0,1
 B01101011,B01111001, // 2,3 
 B00111100,B01011101, // 4,5 
 B01011111,B01110000, // 6,7 
 B01111111,B01111101};// 8,9 

byte I2C_address[8]= 
{0x20,0x21,           //prvni a druhy modul
0x25,0x24,            //treti a ctvrty modul
0x22,0x23,            //paty a sesty modul
0x26,0x27};           //sedmy a osmy modul

LiquidCrystal_I2C lcd(0x3F,16,2);  

RF24 radio(9, 10);  // CE, CSN
const byte addresses [] [6] = {"00001","00002"} ; // adresa komunikatoru

int rozdeleni[2]; //pole pro ulozeni rozdeleni dvoucifernych cisel pro bcd kod (promenna DaM)
int split(int* DaM, int cas);     // funkce, ktera rozdeli dvojciferne cislo na jednotky a desitky
int I2C_7segment_write(int val, int address); // funkce pro zapis jednoho byte na jednotku I2C
void initialize_7segment(); // funkce na inicializaci displeje
void begin_initialize_7segment(); // funkce na pocatecni inicializaci - pomalejsi
void choose_program(); // funkce na vyber programu
void start();

void multiplex_7segment_R();
void multiplex_7segment_L();

void setup() 
{
  Serial.begin(9600);

  display.init();

  lcd.init();  
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" Casomira v 2.0");
  lcd.setCursor(0,1);
  lcd.print("  SDH  JIZBICE");

  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setRetries(4,10);
  radio.setPALevel(HIGH);
  radio.startListening();

  pinMode(TLA, INPUT_PULLUP);        //start
  pinMode(TLB, INPUT_PULLUP);        //stop
  pinMode(TLProg, INPUT_PULLUP);     //prepinac programu (1-stopky 0- odpocet)

  Serial.println("CASOMIRA SDH JIZBICE");
  
  begin_initialize_7segment();  //inicializace 7segmentovek

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" VYBER PROGRAM");
  lcd.setCursor(0,1);
  lcd.print(" STOPKY/ODPOCET");
  delay(1000);
  lcd.clear();

  choose_program();
}

void loop() 
{
  switch (program)
  {
    case 1: //######## STOPKY POZARNI SPORT ########
      
      TERC_P.casSTART = 1;
     /* if(!init_delivery_confirm_bit)
      {
        radio.stopListening();
        if(radio.write(&text1, sizeof(text1)) && !init_delivery_confirm_bit)
        {
          Serial.println("INIT TERCE");
          init_delivery_confirm_bit = HIGH;
          lcd.setCursor(0,0);
          lcd.print(" Cekam na START");
          radio.startListening();
        } 
        /*
        if (!init_delivery_confirm_bit)
        {
          Serial.println("ERROR - CHYBA INITU TERCU");
          lcd.setCursor(0,0);
          lcd.print(" CHYBA SPOJENI");
          lcd.setCursor(0,1);
          lcd.print("     TERCU");
          program = 6;
        }
        */
     // }
     
      if (radio.available())
      {
        radio.read(&text, sizeof(text));
        String ZPRAVA = String(text);
        Serial.println(ZPRAVA);
        if (ZPRAVA == "PRIPRAVEN") READY = HIGH;
        
      }
// SMAZAT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      READY = HIGH;

      if((digitalRead(TLA) == LOW) && READY) //při stisknutí start...
        {
           start();        //skoč na start
        }
    
        while(TERC_P.casSTART > 1)   //pokud bylo odstartováno pak
        {

          if(STOP1 == 0)    //průběžný čas prvního terče
          {
            
            TERC_L.casTERC = millis() - TERC_P.casSTART;
            TERC_L.casTERC_M = TERC_L.casTERC / 60000;
            TERC_L.casTERC_S = (TERC_L.casTERC % 60000)/1000;
            TERC_L.casTERC_ms = TERC_L.casTERC % 1000;
                
            lcd.setCursor(0,0);               //první řádek
            lcd.print("1. ");
            lcd.print(TERC_L.casTERC_M);
            Serial.print("TERC L: ");
            Serial.print(TERC_L.casTERC_M);
            Serial.print(" : ");     
            lcd.print(":");
            Serial.print(TERC_L.casTERC_S);
            Serial.print(" : ");
            lcd.print(TERC_L.casTERC_S);
            lcd.print(":");
            lcd.print(TERC_L.casTERC_ms);
            lcd.print("   ");
            Serial.print(TERC_L.casTERC_ms);
            Serial.print("   ");
    
            multiplex_7segment_L();
                    
          }
              
          if(STOP2 == 0)    //průběžný čas 2.terče
          {
            TERC_P.casTERC = millis() - TERC_P.casSTART;
            TERC_P.casTERC_M = TERC_P.casTERC / 60000;
            TERC_P.casTERC_S = (TERC_P.casTERC % 60000)/1000;
            TERC_P.casTERC_ms = TERC_P.casTERC % 1000;
            lcd.setCursor(0,1);               //druhý řádek
            lcd.print("2. ");
            lcd.print(TERC_P.casTERC_M);
            //Serial.print("TERC P: ");
            //Serial.print(TERC_P.casTERC_M);
            //Serial.print(" : ");
            lcd.print(":");
            lcd.print(TERC_P.casTERC_S);
            //Serial.print(TERC_P.casTERC_S);
            //Serial.print(" : ");
            lcd.print(":");
            lcd.print(TERC_P.casTERC_ms);
            lcd.print("   ");
            //Serial.println(TERC_P.casTERC_ms);
                  
            multiplex_7segment_R();   
    
          }

    
          if(terc1 == HIGH) //konec 1.terče
          {
            Serial.println("L STOP");
            STOP1 = 1;
            TERC_L.casTERC_end = millis() - TERC_P.casSTART;
            TERC_L.casTERC_M = TERC_L.casTERC_end / 60000;  
            TERC_L.casTERC_S = TERC_L.casTERC_end % 60000;  
            TERC_L.casTERC_S = TERC_L.casTERC_S / 1000;     
            TERC_L.casTERC_ms = TERC_L.casTERC_end % 1000;  
    
            multiplex_7segment_L();   //zapsani konecneho casu
                
            lcd.setCursor(0,0);               
            lcd.print("1. ");
                
            if(TERC_L.casTERC_M < 10)      
            {
              lcd.print("0");     
            }
     
            lcd.print(TERC_L.casTERC_M);   
            lcd.print(":");         
                
            if(TERC_L.casTERC_S < 10)      
            {
              lcd.print("0");     
            }
            
            lcd.print(TERC_L.casTERC_S);   
            lcd.print(":");         
            lcd.print(TERC_L.casTERC_ms);  

            terc1 = LOW;
          }
    
          if(terc2 == HIGH) //konec 2. terče
          {
            Serial.println("P STOP");
            STOP2 = 1;
            TERC_P.casTERC_end = millis() - TERC_P.casSTART;
            TERC_P.casTERC_M = TERC_P.casTERC_end / 60000;  //výpočet minut
            TERC_P.casTERC_S = TERC_P.casTERC_end % 60000;  //výpočet sekund
            TERC_P.casTERC_S = TERC_P.casTERC_S / 1000;     //výpočet sekund
            TERC_P.casTERC_ms = TERC_P.casTERC_end % 1000;  //výpočet ms
    
            multiplex_7segment_R();   //zapsani konecneho casu
                
            lcd.setCursor(0,1);               
            lcd.print("2. ");
                
            if(TERC_P.casTERC_M < 10)      
            {
              lcd.print("0");     
            }

            lcd.print(TERC_P.casTERC_M);   
            lcd.print(":");         
                
            if(TERC_P.casTERC_S < 10)      
            {
              lcd.print("0");     
            }
            
            lcd.print(TERC_P.casTERC_S);   
            lcd.print(":");         
            lcd.print(TERC_P.casTERC_ms); 
            terc2 = LOW; 
          }
    
    

          if(digitalRead(TLB) == LOW) //při stisknutí B restart
          {
            TERC_P.casSTART = 1;
            lcd.clear(); 
            initialize_7segment();   //inicializace segmentovek nulami
            choose_program();
            STOP1=0;
            STOP2=0;
            init_delivery_confirm_bit= LOW;

             radio.stopListening();
            if(radio.write(&text2, sizeof(text2)))
            {
            Serial.println("RESET TERCE");
            } 
          }

          if(radio.available())
          {
            char text [32] = {0};
            radio.read(&text,sizeof(text));
            Serial.println(text);

            String ZPRAVA = String(text);
            if (ZPRAVA == "LEVY") terc1 = HIGH;
            if (ZPRAVA == "PRAVY") terc2 = HIGH;
            if (ZPRAVA == "PRAZDNO") prazdno = HIGH;
            Serial.println(ZPRAVA);
            Serial.print("TERC1 ");
            Serial.println(terc1);
            Serial.print("TERC2 ");
            Serial.println(terc2);
            
            radio.writeAckPayload(1, &text, sizeof(text));
            
          
          }
        }
     break;

    case 2: // ######## ODPOCET ########
             
      TERC_P.casSTART = 1;
      lcd.setCursor(0,0);
      lcd.print(" Cekam na start");
      lcd.setCursor(0,1);
      lcd.print("    ODPOCTU");
    
      if(digitalRead(TLA) == LOW) //při stisknutí start...
        {
           start();        //skoč na start
           lcd.clear();
        }
   
        while(TERC_P.casSTART > 1)   //pokud bylo odstartováno pak
        {
               
          TERC_L.casODPOCET =TERC_L.casSTART-millis()+ODPOCET_TERC;
          TERC_L.casTERC_M = TERC_L.casODPOCET / 60000;  //výpočet minut
          TERC_L.casTERC_S = TERC_L.casODPOCET % 60000;  //výpočet sekund
          TERC_L.casTERC_S = TERC_L.casTERC_S / 1000;     //výpočet sekund
          TERC_L.casTERC_ms = TERC_L.casODPOCET % 1000;  //výpočet ms

          lcd.setCursor(0,0);               //první řádek
          lcd.print("    ODPOCET");
          lcd.setCursor(4,1);
          lcd.print(TERC_L.casTERC_M);
          Serial.print("ODPOCET ");
          Serial.print(TERC_L.casTERC_M);
          Serial.print(" : ");     
          lcd.print(":");
          Serial.print(TERC_L.casTERC_S);
          Serial.print(" : ");
          lcd.print(TERC_L.casTERC_S);
          lcd.print(":");
          lcd.print(TERC_L.casTERC_ms);
          lcd.print("   ");
          Serial.print(TERC_L.casTERC_ms);
          Serial.println("   ");

          multiplex_7segment_L();
    
          if((TERC_L.casTERC_M==0)&&(TERC_L.casTERC_S==0))
          {
            program=1;
            TERC_L.casTERC_ms=0;
            lcd.clear();
            initialize_7segment();
            break;               
          }

          if(digitalRead(TLB) == LOW) //při stisknutí B restart
          {
            TERC_P.casSTART = 1;
            lcd.clear(); 
            initialize_7segment();   //inicializace segmentovek nulami
            break;
          }



        }
        case 6: // ERROR - RESET NUTNY
        break;
   break; 
  }
  
}

void start()
{
  TERC_P.casSTART = millis();    //uložení startovního času
  lcd.clear();            //smaž LCD
  radio.startListening(); // zapnuti prijmu radia
}

int split(int* DaM, int cas)  //funkce na rozdeleni dvoucifernych cisel
{
  DaM[0]= cas % 10; //jednotky
  if(((cas - DaM[0])/10)>=0)DaM[1] = (cas - DaM[0])/10; //desitky
  //else DaM[1];

  return 1;
}

void multiplex_7segment_L()   // program ktery zapise na segmentovky cisla ziskana konverzi pomoci funkci - LEVY TERC
{
//########################## MINUTY L ##############################

            split(rozdeleni,TERC_L.casTERC_M);        

            I2C_7segment_write(rozdeleni[0],I2C_address[0]);
            I2C_7segment_write(rozdeleni[1],I2C_address[1]);
            
//########################## SEKUNDY L ##############################
           
            split(rozdeleni,TERC_L.casTERC_S);         

            I2C_7segment_write(rozdeleni[0],I2C_address[2]);
            I2C_7segment_write(rozdeleni[1],I2C_address[3]);        
}

void multiplex_7segment_R()   // program ktery zapise na segmentovky cisla ziskana konverzi pomoci funkci - PRAVY TERC
{
//########################## MINUTY R ##############################

            split(rozdeleni,TERC_P.casTERC_M);            

            I2C_7segment_write(rozdeleni[0],I2C_address[4]);
            I2C_7segment_write(rozdeleni[1],I2C_address[5]);


//########################## SEKUNDY R ##############################
           
            split(rozdeleni,TERC_P.casTERC_S);            

            I2C_7segment_write(rozdeleni[0],I2C_address[6]);
            I2C_7segment_write(rozdeleni[1],I2C_address[7]);
          
}

int I2C_7segment_write(int val, int address)
{
  PCF8574 pcf(address);
  pcf.write8(cislice[val]);
  return 1;
}

void initialize_7segment()
{
  for(int i=0;i<=1;i++)
  {
      for(int i =0;i<=8;i++)
      {
        PCF8574 pcf(I2C_address[i]);
        pcf.begin();
        pcf.write8(cislice[0]);

      }
  }
}

void begin_initialize_7segment()
{
  for(int i=0;i<=1;i++)
  {
      for(int i =0;i<=8;i++)
      {
        PCF8574 pcf(I2C_address[i]);
        pcf.begin();
        pcf.write8(cislice[8]);
        //delay(500);
      }
  }

 // delay (2000);

  for(int i=0;i<=1;i++)
  {
      for(int i =0;i<=8;i++)
      {
        PCF8574 pcf(I2C_address[i]);
        pcf.begin();
        pcf.write8(cislice[0]);
      }
  }
}

void choose_program()
{
  if(digitalRead(TLProg)== HIGH)program=1;
  else
  {
    program=2;
    start();
    lcd.clear();    
  }
}
