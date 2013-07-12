// Original LiquidCrystal library that can be inherited from to support other
// connectivity:
// <a href="https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home" title="https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home" rel="nofollow">https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home</a>
 
#include <LiquidCrystal_SR_LCD3.h>
 
// Standard HD44780 20x4 LCD display driven via 3 output pins to a 4094 8-bit
// shift register like 74HC4094 pebblev2's MC14094
 
// This demo was written for the PebbleV2 to test LiquidCrystal_SR_LCD3, but it
// should work with other LiquidCrystal Displays that are 20x4.
// <a href="http://www.freetronics.com/pages/pebble-v2" title="http://www.freetronics.com/pages/pebble-v2" rel="nofollow">http://www.freetronics.com/pages/pebble-v2</a>
// <a href="https://github.com/lukeweston/pebble20" title="https://github.com/lukeweston/pebble20" rel="nofollow">https://github.com/lukeweston/pebble20</a>

// Data are the input bits
// Clock tells the SR that the data pin holds the next valid bit
// Strobe gets data input to be made visible on the parallel output pins.
 
const int PIN_LCD_STROBE         =  5;  // Out: LCD IC4094 shift-register strobe
const int PIN_LCD_DATA           =  6;  // Out: LCD IC4094 shift-register data
const int PIN_LCD_CLOCK          =  4;  // Out: LCD IC4094 shift-register clock
//const int PIN_LCD_BACKLIGHT      =  5;  // Out: LCD backlight (PWM)
 
// srdata / srclock / strobe
LiquidCrystal_SR_LCD3 lcd(PIN_LCD_DATA, PIN_LCD_CLOCK, PIN_LCD_STROBE);
 
// Creat a set of new characters
byte heart[8] = {
  0b00000, 0b01010, 0b11111, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000
};
 
byte smiley[8] = {
  0b00000, 0b00000, 0b01010, 0b00000, 0b00000, 0b10001, 0b01110, 0b00000
};
 
byte frownie[8] = {
  0b00000, 0b00000, 0b01010, 0b00000, 0b00000, 0b00000, 0b01110, 0b10001
};
 
byte armsDown[8] = {
  0b00100, 0b01010, 0b00100, 0b00100, 0b01110, 0b10101, 0b00100, 0b01010
};
 
byte armsUp[8] = {
  0b00100, 0b01010, 0b00100, 0b10101, 0b01110, 0b00100, 0b00100, 0b01010
};
byte man_with_hat[8] =
{
  0b01110,
  0b11111,
  0b00000,
  0b01010,
  0b00100,
  0b10001,
  0b01110,
  0b00000,
};
void setup(){
    //pinMode(PIN_LCD_BACKLIGHT, OUTPUT);
    //analogWrite(PIN_LCD_BACKLIGHT, 80);
 
    lcd.begin(16, 2);               // initialize the lcd
 
    // load characters to the LCD
    lcd.createChar(0, smiley);
    lcd.createChar(1, frownie);
    lcd.createChar(2, armsDown); 
    lcd.createChar(3, armsUp); 
    lcd.createChar(4, heart);
    lcd.createChar(5,man_with_hat);
 
    lcd.home ();                   // go home
    lcd.setCursor (0, 0);
    //lcd.print(F("LiquidCrystal_SRLCD3")); 
    //lcd.setCursor (0, 1);
    //lcd.print(F("he.."));
    //lcd.setCursor (0, 2);
    //lcd.print(F("..lo"));
    //lcd.setCursor (0, 3);
    //lcd.print(F("World "));
    lcd.print(char(4));
    lcd.print(char(2));
    lcd.print(char(3));
    lcd.print(char(1));
    lcd.print(char(0));
    lcd.print(char(5));
     
}
 
void loop()
{
   
}
