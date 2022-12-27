/*
Copyright (c) 2021 Jakub Mandula

Example of setting a custom ModBUS address for the PZEM modules.
================================================================

Each PZEM modules has two addresses. A default general address (0xF8),
which every module will listen on by default. And a custom address.

The custom address can be used if multiple PZEM modules are to be used
on a single ModBUS.

This script will use the general address in order to set the custom 
address of the connected PZEM module.

Therefore make sure only one PZEM module is connected at a time! 
Otherwise all connected modules will receive the same custom address.

*/

#include <PZEM004Tv30.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

  int addr=1;
  
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/


 return btnNONE;  // when all others fail, return this...
}


#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN 0
#define PZEM_TX_PIN 1
#endif


#define PZEM_SERIAL Serial



PZEM004Tv30 pzem(PZEM_SERIAL);



/*******************************************
 * Set your address over here..
 * The address can be between 0x01 and 0xF7
 *******************************************/

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {

//    static uint8_t addr = SET_ADDRESS;
lcd.setCursor(0,0);
    // Print out current custom address
    lcd.print("Old address:0x");
    lcd.print(pzem.readAddress(), HEX); lcd.print("  ");
lcd.setCursor(0,1);
    // Set the custom address
    lcd.print("Select for 0x");
    lcd.print(addr, HEX); lcd.print("  ");
    
    //
 /*   if (0==0)
    
    */
lcd_key = read_LCD_buttons();  // read the buttons
lcd.setCursor(0,1);
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     addr=addr+16;
     if (addr>128) addr=128;
     lcd.print("Select for 0x");
    lcd.print(addr, HEX); lcd.print("  ");
     delay(500);
     break;
     }
   case btnLEFT:
     {
     addr=addr-16;
     if (addr<1) addr=1;
     lcd.print("Select for 0x");
    lcd.print(addr, HEX); lcd.print("  ");
     delay(500);
     break;
     }
   case btnUP:
     {
     addr++;
     if (addr>128) addr=128;
   lcd.print("Select for 0x");
    lcd.print(addr, HEX); lcd.print("  ");
     delay(500);
     break;
     }
   case btnDOWN:
     {
     addr--;
     if (addr<1) addr=1;
    lcd.print("Select for 0x");
    lcd.print(addr, HEX); lcd.print("  ");
     delay(500);
     break;
     }
   case btnSELECT:
     {
     if(!pzem.setAddress(addr))
     {
      // Setting custom address failed. Probably no PZEM connected
      lcd.setCursor(0,0);
      lcd.print("Error setting   ");
      lcd.setCursor(0,1);
      lcd.print("address.        ");
    }
    else
    {
      lcd.setCursor(0,0);
      lcd.print("Setting adress  ");
      lcd.setCursor(0,1);
      lcd.print("successfull     ");
    }
    delay(2000);
     break;
     }
     case btnNONE:
     {
     break;
     }
 }

   
}

    
