#include <S65Display.h>
S65Display lcd;

void setup()
{
  //init LCD
  lcd.init(4); //spi-clk = Fcpu/4
  //clear screen
  lcd.clear(RGB(255,0,0));
  
}



void loop()
{
  lcd.drawLine(10, 10, 50, 40, RGB(  0,255,255));
  lcd.drawLine(10, 40, 50, 10, RGB(  0,255,255));

  lcd.drawCircle(88, 30, 12, RGB(255,255,  0));

  lcd.drawTextPGM(10, 50, PSTR("Pasha")          , 3, RGB(  0,255,  0), RGB(255,  0,  0));
  lcd.drawTextPGM(10, 80, PSTR("         by         "), 1, RGB(255,255,  0), RGB(255,  0,  0));
  lcd.drawTextPGM(10, 95, PSTR("Watterott electronic"), 1, RGB(255,255,255), RGB(255,  0,  0));
  lcd.drawTextPGM(10,110, PSTR(" www.watterott.com  "), 1, RGB(  0,  0,255), RGB(255,  0,  0));
 
  delay(1000);
} 
