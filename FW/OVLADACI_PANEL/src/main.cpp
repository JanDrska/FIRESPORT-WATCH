//####### PROGRAM PRO OVLADACI PULT HASICSKYCH STOPEK  ######
// PROGRAM VYTVORIL JAN DRSKA PRO JEDNOTKU SDH JIZBICE
/*
              /----/  /----/        /----/  /----/
             /    /  /    /  /--/  /    /  /    /
            /----/  /----/        /----/  /----/
           /    /  /    /  /--/  /    /  /    /
          /----/  /----/        /----/  /----/
*/

// VERZE 1.0
// POSLEDNI UPRAVA 03.04.2022

#include <LcdMenu.h>
#include "sdhCmd.h"

#define CLK 8
#define DT 9
#define SW 2

#define LCD_ROWS 2
#define LCD_COLS 16

// Configure keyboard keys (ASCII)
#define UP 56        // NUMPAD 8
#define DOWN 50      // NUMPAD 2
#define LEFT 52      // NUMPAD 4
#define RIGHT 54     // NUMPAD 6
#define ENTER 53     // NUMPAD 5
#define BACK 55      // NUMPAD 7
#define BACKSPACE 8  // BACKSPACE
#define CLEAR 46     // NUMPAD .

//SdhCmd pozarniSport;
//SdhCmd vycvik;
//SdhCmd odpocet;

bool menuZpetPom = false;

// Declare the main menu
extern MenuItem mainMenu[];
extern MenuItem Mpozarnisport[];
extern MenuItem Mvycvik[];
extern MenuItem Modpocet[];
extern MenuItem Mskore[];
extern MenuItem Mhodiny[];
extern MenuItem Mnastaveni[];

// Definice hlavniho menu
MenuItem mainMenu[] = {ItemHeader(),
                       ItemSubMenu("Pozarni sport",Mpozarnisport),
                       ItemSubMenu("Vycvik 5+1",Mvycvik),
                       ItemSubMenu("Odpocet",Modpocet),
                       ItemSubMenu("Skore",Mskore),
                       ItemSubMenu("Hodiny",Mhodiny),
                       ItemSubMenu("Nastaveni",Mnastaveni),
                       ItemFooter()};
// Definice vsech submenu 1. kategorie

void menuZpet();
void pozarniUtokStartPovolen(uint8_t isOn);
void pozarniUtokStart();
void pozarniUtokStop();
void pozarniUtokReset();
void pozarniUtokVypustit();
void pozarniUtokResetTercu();

MenuItem Mpozarnisport[] =  
{
    ItemHeader(mainMenu),
    ItemCommand("Zpet",menuZpet),
    ItemCommand("Start",pozarniUtokStart),
    ItemCommand("Stop",pozarniUtokStop),
    ItemCommand("Restart",pozarniUtokReset),
    ItemCommand("Vypustit terce",pozarniUtokVypustit),
    ItemCommand("Reset tercu",pozarniUtokResetTercu),
    ItemFooter()};

void vycvikStart();
void vycvikStop();
void vycvikReset();

MenuItem Mvycvik[] = 
{
    ItemHeader(mainMenu),
    ItemCommand("Zpet",menuZpet), 
    ItemCommand("Start",vycvikStart),
    ItemCommand("Stop",vycvikStop),
    ItemCommand("Reset",vycvikReset),
    ItemFooter()};

void odpocetStart();
void odpocetStop();
void odpocetReset(); 

MenuItem Modpocet[] = 
{
    ItemHeader(mainMenu),
    ItemCommand("Zpet",menuZpet), 
    ItemCommand("Start",odpocetStart),
    ItemCommand("Stop",odpocetStop), 
    ItemCommand("Reset",odpocetReset), 
    ItemFooter()};

void skoreT1p();
void skoreT2p(); 
void skoreT1m();
void skoreT2m();
void skoreStart();
void skoreReset();

MenuItem Mskore[] = 
{
    ItemHeader(mainMenu),
    ItemCommand("Zpet",menuZpet), 
    ItemCommand("Tym 1 +",skoreT1p),
    ItemCommand("Tym 2 +",skoreT2p), 
    ItemCommand("Tym 1 -",skoreT1m),
    ItemCommand("Tym 2 -",skoreT2m), 
    ItemCommand("Start",skoreStart),
    ItemCommand("Reset jednotky",skoreReset),
    ItemFooter()};

void hodinyStart();
void hodinyStop();
void hodinyNastavit();
void hodinyRezim();

MenuItem Mhodiny[] = 
{
    ItemHeader(mainMenu),
    ItemCommand("Zpet",menuZpet), 
    ItemCommand("Spustit",hodinyStart),
    ItemCommand("Zastavit",hodinyStop), 
    ItemCommand("Nastavit",hodinyNastavit),
    ItemCommand("Rezim",hodinyRezim), 
    ItemFooter()};

void nastaveniReset();
void nastaveniPodsviceni(uint8_t isOn);

MenuItem Mnastaveni[] = 
{
    ItemHeader(mainMenu),
    ItemCommand("Zpet",menuZpet), 
    ItemCommand("Reset",nastaveniReset),
    ItemToggle("Podsviceni",nastaveniPodsviceni), 
    ItemFooter()};

LcdMenu menu(LCD_ROWS, LCD_COLS);

int encCounter = 0;

  bool btn = 0;
  int counter = 0;
  int actualStateCLK = 0;
  int lastStateCLK = 0;
  unsigned long lastButtonPressTime = 0;

void setup() 
{
    Serial.begin(9600);
    menu.setupLcdWithMenu(0x3F, mainMenu);
    pinMode(CLK,INPUT_PULLUP);
    pinMode(DT,INPUT_PULLUP);
    pinMode(SW,INPUT_PULLUP);
    lastStateCLK = digitalRead(CLK);
}

void loop() 
{
    actualStateCLK = digitalRead(CLK);

	if (actualStateCLK != lastStateCLK  && actualStateCLK == 1)
    {
	    if (digitalRead(DT) != actualStateCLK)
        {
            if(counter > -10)
                counter --;
        }
    
		else 
        {
            if(counter < 10)
                counter ++;
        }
        
        Serial.print(counter);
	}

	lastStateCLK = actualStateCLK; 
	int btnState = digitalRead(SW); 

	if (btnState == LOW) 
    {
		if (millis() - lastButtonPressTime > 150) 
            btn = HIGH;
        else 
            btn = LOW;
            
		lastButtonPressTime = millis(); 
	}

    if (counter > encCounter) 
        menu.up();
    if (counter < encCounter)
        menu.down();
    
    encCounter = counter;

    if(btn == HIGH)
        menu.enter();

    if (menuZpetPom)
    {
        menu.back();
        menuZpetPom = false;
    }



}

void menuZpet()
{
    menuZpetPom = true;
}

void pozarniUtokStartPovolen(uint8_t isOn){}
void pozarniUtokStart(){Serial.write(10);}
void pozarniUtokStop(){Serial.write(11);}
void pozarniUtokReset(){Serial.write(12);}
void pozarniUtokVypustit(){Serial.write(13);}
void pozarniUtokResetTercu(){Serial.write(14);}

void vycvikStart(){Serial.write(20);}
void vycvikStop(){Serial.write(21);}
void vycvikReset(){Serial.write(22);}

void odpocetStart(){Serial.write(30);}
void odpocetStop(){Serial.write(31);}
void odpocetReset(){Serial.write(32);}

void skoreT1p(){Serial.write(40);}
void skoreT2p(){Serial.write(41);} 
void skoreT1m(){Serial.write(42);}
void skoreT2m(){Serial.write(43);}
void skoreStart(){Serial.write(44);}
void skoreReset(){Serial.write(45);}

void hodinyStart(){Serial.write(50);}
void hodinyStop(){Serial.write(51);}
void hodinyNastavit(){Serial.write(52);}
void hodinyRezim(){Serial.write(53);}

void nastaveniReset(){Serial.write(60);}
void nastaveniPodsviceni(uint8_t isOn){menu.toggleBacklight();}