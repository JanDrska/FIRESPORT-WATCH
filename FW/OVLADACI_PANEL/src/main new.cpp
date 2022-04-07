//####### PROGRAM PRO OVLADACI PULT HASICSKYCH STOPEK  ######
// PROGRAM VYTVORIL JAN DRSKA PRO JEDNOTKU SDH JIZBICE
// PROGRAM JE VYHRADNIM VLASTNICTVIM AUTORA A SDH JIZBICE - JAKEKOLIV SIRENI PROGRAMU BEZ VEDOMI AUTORA JE ZAPOVEZENO!!!
/*
              /----/  /----/        /----/  /----/
             /    /  /    /  /--/  /    /  /    /
            /----/  /----/        /----/  /----/
           /    /  /    /  /--/  /    /  /    /
          /----/  /----/        /----/  /----/
*/

// VERZE 3.0
// POSLEDNI UPRAVA 07.04.2022


#include "CMBMenu.hpp"  //Copyright (C) 2022  Michael Bernhard
#include "LiquidCrystal_I2C.h"

// Externi klavesnice
#define E_Start A0
#define E_Stop A1
#define E_Reset A2
#define E_Enable A3

// Enkoder
#define ENC_BTN 2
#define OutputA 8
#define OutputB 9

// Externi vstupy start
#define EXT_IN1 3 // moznost vyuziti preruseni
#define EXT_IN2 4

// Indikacni LED
#define LED_READY 7
#define LED_R 10
#define LED_L 13
#define LED_EMPTY 11
#define LED_RUN 12

// define text to display
const char menuPozSport[] PROGMEM = {"Pozarni sport"};  //Hlavni nabidka
const char menuVycvik[] PROGMEM = {"Vycvik 5+1"};       //Hlavni nabidka
const char menuOdpocet[] PROGMEM = {"Odpocet"};         //Hlavni nabidka
const char menuSkore[] PROGMEM = {"Pocitadlo skore"};   //Hlavni nabidka
const char menuHodiny[] PROGMEM = {"Hodiny"};           //Hlavni nabidka
const char menuNastaveni[] PROGMEM = {"Nastaveni"};     //Hlavni nabidka

const char menuPsZpet[] PROGMEM = {"Zpet"};             // Submenu - pozarni sport
const char menuPsStart[] PROGMEM = {"Start"};           // Submenu - pozarni sport
const char menuPsStop[] PROGMEM = {"Stop"};             // Submenu - pozarni sport
const char menuPsRestart[] PROGMEM = {"Restart"};       // Submenu - pozarni sport
const char menuVypustit[] PROGMEM = {"Vypustit terce"}; // Submenu - pozarni sport
const char menuResetTercu[] PROGMEM = {"Reset tercu"};  // Submenu - pozarni sport

const char menuVyZpet[] PROGMEM = {"Zpet"};             // Submenu - vycvik 5+1
const char menuVyStart[] PROGMEM = {"Start"};           // Submenu - vycvik 5+1
const char menuVyStop[] PROGMEM = {"Stop"};             // Submenu - vycvik 5+1
const char menuVyRestart[] PROGMEM = {"Restart"};       // Submenu - vycvik 5+1

const char menuOdZpet[] PROGMEM = {"Zpet"};             // Submenu - odpocet
const char menuOdStart[] PROGMEM = {"Start"};           // Submenu - odpocet
const char menuOdStop[] PROGMEM = {"Stop"};             // Submenu - odpocet
const char menuOdReset[] PROGMEM = {"Reset"};           // Submenu - odpocet

const char menuHoZpet[] PROGMEM = {"Zpet"};             // Submenu - hodiny
const char menuHoStart[] PROGMEM = {"Spustit"};         // Submenu - hodiny
const char menuHoStop[] PROGMEM = {"Zastavit"};         // Submenu - hodiny
const char menuHoNastavit[] PROGMEM = {"Nastavit"};     // Submenu - hodiny
const char menuHoRezim[] PROGMEM = {"Rezim"};           // Submenu - hodiny

const char menuNaZpet[] PROGMEM = {"Zpet"};             // Submenu - nastaveni
const char menuNaReset[] PROGMEM = {"Reset"};           // Submenu - nastaveni
const char menuNaPodsviceni[] PROGMEM = {"Podsviceni"}; // Submenu - nastaveni

// define function IDs
enum MenuFID {
  MenuDummy,MenuPozSport, MenuPsZpet, MenuPsStart, MenuPsStop, MenuPsReset, MenuPsVypustit, MenuPsResetTercu,
  MenuVycvik, 
  MenuOdpocet,
  MenuSkore, 
  MenuHodiny, 
  MenuNastaveni,
  MenuBar, 
  MenuBarA
};

// define key types

enum KeyType {
  KeyNone, // no key is pressed
  KeyLeft,
  KeyRight,
  KeyEnter,
  KeyExit,
};

KeyType getKey();

void printMenuEntry(const char* f_Info);

// (here for maximum 100 menu entries)
CMBMenu<100> menu;
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();

  Serial.begin(9600);

  // ** menu **
  // add nodes to menu (layer, string, function ID)
  menu.addNode(0, menuPozSport , MenuPozSport);
  menu.addNode(1, menuPsZpet, MenuPsZpet);
  menu.addNode(1, menuPsStart, MenuPsStart);
  menu.addNode(1, menuPsStop, MenuPsStop);
  menu.addNode(1, menuPsRestart, MenuPsReset);
  menu.addNode(1, menuResetTercu, MenuPsResetTercu);
  menu.addNode(1, menuVypustit, MenuPsVypustit);

  menu.addNode(0, menuVycvik, MenuVycvik);

  menu.addNode(0, menuOdpocet, MenuOdpocet);

  menu.addNode(0, menuSkore, MenuSkore);

  menu.addNode(0, menuHodiny, MenuHodiny);

  menu.addNode(0, menuNastaveni, MenuBar);

  //menu.addNode(1, g_MenuBarA_pc, MenuBarA);


  // ** menu **
  // build menu and print menu
  // (see terminal for output)
  const char* info;
  menu.buildMenu(info);
  menu.printMenu();

  // ** menu **
  // print current menu entry
   printMenuEntry(info);
}

void loop()
{
  // function ID
  int fid = 0;

  // info text from menu
  const char* info;

  // go to deeper or upper layer?
  bool layerChanged=false;

  // determine pressed key
  KeyType key = getKey();

  // ** menu **
  // call menu methods regarding pressed key
  switch(key) {
    case KeyExit:
      menu.exit();
      break;
    case KeyEnter:
      menu.enter(layerChanged);
      break;
    case KeyRight:
      menu.right();
      break;
    case KeyLeft:
      menu.left();
      break;
    default:
      break;
  }

  // ** menu **
  // pint/update menu when key was pressed
  // and get current function ID "fid"
  if (KeyNone != key) {
    fid = menu.getInfo(info);
    printMenuEntry(info);
  }

  // ** menu **
  // do action regarding function ID "fid"
  if ((0 != fid) && (KeyEnter == key) && (!layerChanged)) {
    switch (fid) 
    {
      case MenuPozSport:
       // FooA();
        break;
      case MenuVycvik:
        //BarA();
        break;
      case MenuOdpocet:
        //Test1();
        break;
      case MenuSkore:
        //Test2();
        break;
      default:
        break;
    }
  }
}

// ********************************************
// ** menu **
// printMenuEntry
// ********************************************
void printMenuEntry(const char* f_Info)
{
  String info_s;
  MBHelper::stringFromPgm(f_Info, info_s);

  // when using LCD: add/replace here code to
  // display info on LCD
  Serial.println("----------------");
  Serial.println(info_s);
  Serial.println("----------------");

  // print on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(info_s);

  // you can print here additional infos into second line of LCD
  // lcd.setCursor(0, 1);
  // lcd.print("my text");
}


// ********************************************
// getKey
// ********************************************
KeyType getKey()
{
  KeyType key = KeyNone;
  //Serial.println("KEY");

  // here for demonstration: get "pressed" key from terminal
  // replace code when using push buttons
  while(Serial.available() > 0) {
    String Key_s = Serial.readString();
    Key_s.trim();
    Serial.println("");
    if(Key_s.equals("l")) { // left
      key = KeyLeft;
      Serial.println("<left>");
    } else if (Key_s.equals("r")) { // right
      key = KeyRight;
      Serial.println("<right>");
    } else if (Key_s.equals("e")) { // enter
      key = KeyEnter;
      Serial.println("<enter>");
    } else if (Key_s.equals("x")) { // exit
      key = KeyExit;
      Serial.println("<exit>");
    } else if (Key_s.equals("m")) { // print menu
      menu.printMenu();
    }
  }

 return key;
}

// ********************************************
// FooA
// ********************************************
void FooA()
{
  Serial.println("Function FooA() was called.");
  lcd.setCursor(0,1);
  lcd.print("FooA()    ");
}

// ********************************************
// Test1
// ********************************************
void Test1()
{
  Serial.println("Function Test1() was called.");
  lcd.setCursor(0,1);
  lcd.print("Test1()   ");
}

// ********************************************
// Test2
// ********************************************
void Test2()
{
  Serial.println("Function Test2() was called.");
  lcd.setCursor(0,1);
  lcd.print("Test2()   ");
}

// ********************************************
// BarA
// ********************************************
void BarA()
{
  Serial.println("Function BarA() was called.");
  lcd.setCursor(0,1);
  lcd.print("BarA()    ");
}

// ========================================================================
// naming convention
// prefix:
//   g  : global variable
//   f  : function/method variable
//   m  : private member of class
//      : without prefix: public member of class
//
//
// ========================================================================