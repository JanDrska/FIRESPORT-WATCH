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

// Declare the main menu
extern MenuItem mainMenu[];
extern MenuItem pozarnisport[];
extern MenuItem vycvik[];
extern MenuItem odpocet[];
extern MenuItem skore[];
extern MenuItem hodiny[];
extern MenuItem nastaveni[];

void back();

// Define the main menu
MenuItem mainMenu[] = {ItemHeader(),
                       ItemSubMenu("Pozarni sport",pozarnisport),
                       ItemSubMenu("5+1",vycvik),
                       ItemSubMenu("Odpocet",odpocet),
                       ItemSubMenu("Pocitadlo skore",skore),
                       ItemSubMenu("Hodiny",hodiny),
                       ItemSubMenu("Nastaveni",nastaveni),
                       ItemFooter()};
/**
 * Create submenu and precise its parent
 */
MenuItem pozarnisport[] = {ItemHeader(mainMenu), MenuItem("Start"),
                           MenuItem("Stop"), ItemFooter()};

MenuItem vycvik[] = {ItemHeader(mainMenu), MenuItem("Start"),
                           MenuItem("Stop"), ItemFooter()};

MenuItem odpocet[] = {ItemHeader(mainMenu), MenuItem("Start"),
                           MenuItem("Reset"), ItemFooter()};

MenuItem skore[] = {ItemHeader(mainMenu), MenuItem("Domaci"),
                           MenuItem("Hoste"), ItemFooter()};

MenuItem hodiny[] = {ItemHeader(mainMenu), MenuItem("Nastavit"),
                           MenuItem("Rezim"), ItemFooter()};

MenuItem nastaveni[] = {ItemHeader(mainMenu), MenuItem("Reset"),
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

void back()
{
    menu.toggleBacklight();
    menu.up();
}