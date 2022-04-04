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

SdhCmd pozarniUtok;
SdhCmd vycvik;
SdhCmd odpocet;

// Declare the main menu
extern MenuItem mainMenu[];
extern MenuItem Mpozarnisport[];
extern MenuItem Mvycvik[];
extern MenuItem Modpocet[];
extern MenuItem Mskore[];
extern MenuItem Mhodiny[];
extern MenuItem Mnastaveni[];

void timerStartEnable();
void prdel();

// Definice hlavniho menu
MenuItem mainMenu[] = {ItemHeader(),
                       ItemSubMenu("Pozarni sport",Mpozarnisport),
                       ItemSubMenu("Vycvik 5+1",Mvycvik),
                       ItemSubMenu("Odpocet",Modpocet),
                       ItemSubMenu("Pocitadlo skore",Mskore),
                       ItemSubMenu("Hodiny",Mhodiny),
                       ItemSubMenu("Nastaveni",Mnastaveni),
                       ItemFooter()};
// Definice vsech submenu 1. kategorie

MenuItem Mpozarnisport[] =   {ItemHeader(mainMenu),MenuItem("Povolit start"),
                            MenuItem("Start"), MenuItem("Stop"),
                            MenuItem("Restart"),MenuItem("Vypustit terce"),
                            MenuItem("Reset tercu"),ItemFooter()};

MenuItem Mvycvik[] = {ItemHeader(mainMenu), MenuItem("Start"),
                    MenuItem("Stop"),MenuItem("Reset"),
                    ItemFooter()};

MenuItem Modpocet[] = {ItemHeader(mainMenu), MenuItem("Start"),
                     MenuItem("Stop"), MenuItem("Reset"), 
                     ItemFooter()};

MenuItem Mskore[] = {ItemHeader(mainMenu), MenuItem("Tym 1 +"),
                           MenuItem("Tym 2 +"), MenuItem("Tym 1 -"),
                           MenuItem("Tym 2 -"), MenuItem("Reset jednotky"),
                           ItemFooter()};

MenuItem Mhodiny[] = {ItemHeader(mainMenu), MenuItem("Spustit"),
                    MenuItem("Zastavit"), MenuItem("Nastavit"),
                    MenuItem("Rezim"), ItemFooter()};

MenuItem Mnastaveni[] = {ItemHeader(mainMenu), MenuItem("Reset"),
                           MenuItem("Podsviceni"), ItemFooter()};


LcdMenu menu(LCD_ROWS, LCD_COLS);

void setup() 
{
    Serial.begin(9600);
    menu.setupLcdWithMenu(0x3F, mainMenu);
}

void loop() 
{
    if (!Serial.available()) return;
    char command = Serial.read();

    if (command == UP)
        menu.up();
    else if (command == DOWN)
        menu.down();
    else if (command == LEFT)
        menu.left();
    else if (command == RIGHT)
        menu.right();
    else if (command == ENTER)
        menu.enter();
    else if (command == BACK)
        menu.back();
    else if (command == CLEAR)
        menu.clear();
    else if (command == BACKSPACE)
        menu.backspace();
    else
        menu.type(command);
}

void prdel()
{
    
}