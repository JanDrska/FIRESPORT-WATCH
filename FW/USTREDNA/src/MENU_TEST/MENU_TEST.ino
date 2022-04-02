
//LCD config
#include <Wire.h> 
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);   //sometimes the LCD adress is not 0x3f. Change to 0x27 if it dosn't work.


//Variables for the menu encoder
int counter = 0; 
int page=1;
int Ready=0;
int submenu=0;
int pushed = 0;
int aState;
int aLastState; 
int last_counter=0; 

uint8_t arrow[8] = {0x00, 0x04 ,0x06, 0x1f, 0x06, 0x04, 0x00};

#define push 10
#define outputA 8
#define outputB 9

void setup()
{
  pinMode (push,INPUT_PULLUP);       //Define the pin as input
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  pinMode (5,OUTPUT);
  digitalWrite(5,HIGH);


  aLastState = digitalRead(outputA);  
  
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight
  lcd.createChar(1, arrow);   //Create the arrow symbol
  lcd.home();                 //Home the LCD
  
  Serial.begin(9600);  

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(1);  
  lcd.print("SDH JIZBICE");
  lcd.setCursor(0,1);  
  lcd.print("V1");

  delay(1000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Vyber sport");
  lcd.setCursor(0,1);  
  lcd.write(1);  
  lcd.print("Pozarni sport");
}

void loop()
{ 
  
  if((last_counter > counter) || (last_counter < counter)  || pushed) // LCD MENU se zobrazi pouze při změně stavu encoderu nebo stisknuti tlacitka
  {
    Ready=1;
    
    //SUBMENU - VOLBA SPORTU
    if(submenu == 0)
    {  
      if(counter == 0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Vyber sport");
        lcd.setCursor(0,1);  
        lcd.write(1);  
        lcd.print("Pozarni sport");
        page=1;
        Serial.println("SUBMENU 0");
        if(pushed)
        {
          pushed=0;
          Serial.println("PUSHED");
        } 
      }
    
      if(counter == 1)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Vyber sport");
        lcd.setCursor(0,1);
        lcd.write(1);   
        lcd.print("5+1");
        page=2; 
        if(pushed)
        {
          pushed=0;
        }   
      }
      
      if(counter ==2)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Vyber sport");
        lcd.setCursor(0,1);  
        lcd.write(1);  
        lcd.print("Odpocet");
        page=3; 
        if(pushed)
        {
          pushed=0;
        }   
      }
    
      if(counter ==3 )
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Vyber sport");
        lcd.setCursor(0,1);
        lcd.write(1);   
        lcd.print("Pocitadlo skore");
        page=4;
        if(pushed)
        {
          pushed=0;
        }  
      }
    }//submenu = 0;


    //SUBMENU - POZARNI SPORT
    if(submenu == 1)
      {  
        if(counter ==0)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1);  
          lcd.print("Povolit start");
          lcd.setCursor(0,1);  
          lcd.print(" Start");
          page=1;
          pushed=0;   
          Serial.println("SUBMENU 1,1"); 
        }
      
        if(counter ==1)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1);   
          lcd.print("Start");
          lcd.setCursor(0,1);
          lcd.print(" Reset");
          page=2;
          pushed=0;  
          Serial.println("SUBMENU 1,2");    
        }
      
        if(counter ==2)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1);  
          lcd.print("Reset");
          lcd.setCursor(0,1);  
          lcd.print(" Stop");
          page=3;
          pushed=0;  
          Serial.println("SUBMENU 1,3");    
        }
      
        if(counter ==3)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1);   
          lcd.print("Stop");
          lcd.setCursor(0,1);
          lcd.print(" Vypustit terce");
          page=4;
          pushed=0;    
          Serial.println("SUBMENU 1,4");    
        }
        if(counter ==4)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.write(1);   
          lcd.print("Vypustit terce");
          lcd.setCursor(0,1);
          lcd.print(" Stav tercu");
          page=5;
          pushed=0;        
          Serial.println("SUBMENU 1,5");
        }
        if(counter ==5)
        {
          lcd.clear();
          lcd.setCursor(0,0);   
          lcd.print(" Vypustit terce");
          lcd.setCursor(0,1);
          lcd.write(1);
          lcd.print("Stav tercu");
          page=6;
          pushed=0;     
          Serial.println("SUBMENU 1,6");   
        }
      }//submenu = 1;

    //SUBMENU Technicky vycvik druzstva 5+1
    if(submenu == 2)
    {  
      if(0 <= counter && counter < 5)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);  
        lcd.print("Povolit start");
        lcd.setCursor(0,1);  
        lcd.print(" Start");
        page=1;
        pushed=0;    
      }
    
      if(5 < counter && counter < 10)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);   
        lcd.print("Start");
        lcd.setCursor(0,1);
        lcd.print(" Stop");
        page=2;
        pushed=0;      
      }
    
      if(10 < counter && counter < 15)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);  
        lcd.print("Stop");
        lcd.setCursor(0,1);  
        lcd.print(" Reset");
        page=3;
        pushed=0;      
      }
    
      if(15 < counter && counter < 20)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);   
        lcd.print("Reset");
        lcd.setCursor(0,1);
        page=4;
        pushed=0;        
      }
    }//submenu = 2;


    //SUBMENU Odpocet
    if(submenu == 3)
    {  
      
      if(0 <= counter && counter < 5)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);   
        lcd.print("Start");
        lcd.setCursor(0,1);
        lcd.print(" Stop");
        page=1;
        pushed=0;      
      }
    
      if(5 < counter && counter < 10)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);  
        lcd.print("Stop");
        lcd.setCursor(0,1);  
        lcd.print(" Reset");
        page=2;
        pushed=0;      
      }
    
      if(10 < counter && counter < 15)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);   
        lcd.print("Reset");
        lcd.setCursor(0,1); 
        lcd.print(" Nastaveni casu");
        page=3;
        pushed=0;
      }  

       if(15 < counter && counter < 20)
       {
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.write(1);  
         lcd.print("Nastaveni casu");
         page=4;
         pushed=0;    
       }      
     }//submenu = 3;

  
    if(submenu == 4)
    {  
      if(0 <= counter && counter < 5)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);  
        lcd.print("Vynulovat");
        lcd.setCursor(0,1);  
        lcd.print(" Odečíst bod D");
        page=1;
        pushed=0;    
      }
    
      if(5 < counter && counter < 10)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);   
        lcd.print("Odečíst bod D");
        lcd.setCursor(0,1);
        lcd.print(" Odečíst bod H");
        page=2;
        pushed=0;      
      }
    
      if(10 < counter && counter < 15)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write(1);  
        lcd.print(" Odečíst bod H");
        page=3;
        pushed=0;      
      }
    }//submenu = 4;
  }

  last_counter = counter; //ulozeni posledni hodnoty citace

  if(!digitalRead(push))
  {
    if(submenu == 1)
    {    
      if(page==1)
      {
      submenu=0;
      counter=1;
      pushed=0;
      Ready=0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PROVADIM");
      lcd.setCursor(0,1);  
      lcd.print("PROSIM CEKEJTE");
      //lcd.clear();
      //pozarni_sport();
      Serial.println("SUBMENU 1 str1");
      delay(4000);
      }
    
      if(page==2)
      {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        //lcd.clear();
        //5+1();
        Serial.println("SUBMENU 1 str2");
       }
    
      if(page==3)
      {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        //lcd.clear();
        //odpocet();
        Serial.println("SUBMENU 1 str3");
        delay(500);
      }
    
       if(page==4)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        //lcd.clear();
        //skore();
        Serial.println("SUBMENU 1 str4");
        delay(500);
       }
    }//end of submenu 1

    if(submenu == 2)
    {    
       if(page==1)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0; 
        delay(500);
        //lcd.clear();
        //povolit_start();
       }
    
       if(page==2)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //start();
       }
    
       if(page==3)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //reset();
       }
    
       if(page==4)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //stop();
       }

       if(page==5)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //vypustit_terce();
       }

       if(page==6)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //stav_tercu();
       }
    }//end of submenu 2

    if(submenu == 3)
    {    
       if(page==1)
       {
        submenu=0;
        counter=1;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //start_odpoctu();
       }
    
       if(page==2)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //stop();
       }  

       if(page==2)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //reset();
       }  

       if(page==2)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //nastaveni_casu();
       }    
    }//end of submenu 3

    if(submenu == 4)
    {    
       if(page==1)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //vynulovat_score();  
       }
    
       if(page==2)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //Domaci_minus();
       }
    
       if(page==3)
       {
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        delay(500);
        //lcd.clear();
        //Hoste_minus();  
       }
    }//end of submenu 4
  

  if(submenu == 0 && Ready==1)
    {    
       if(page==1)
       {
        submenu=1;
        counter=0;
        pushed=1;
        delay(500);
        Serial.println("SUBMENU1 & READY");
       }
    
       if(page==2)
       {
        submenu=2;
        counter=0;
        pushed=1;
        delay(500);
       }
    
       if(page==3)
       {
        submenu=3;
        counter=0;
        pushed=1;
        delay(500);
       }
    
       if(page==4)
       {
        submenu=4;
        counter=0;
        pushed=1;
        delay(500);
       }
    }//end of submenu 0
  }

  aState = digitalRead(outputA); // Reads the "current" state of the outputA

   if (aState != aLastState)
   {     
     if (digitalRead(outputB) != aState)
     {
      if(counter <30)counter ++;
     }
     else
     {
      if (counter > 0) counter --;
      else counter = 0;
     }
     
     Serial.print("Position: ");
     Serial.println(counter);
   } 

  aLastState = aState;

}
