/*
ls020 pinout

LED-          GND
LED+          +9v
1V8           n/c
GND           GND
2V9           +3,3v
DAT   MOSI    pin 11
CLK   SCK     pin 13
CS            pin A4
RESET         pin A3
RS            pin 4
*/

#include <S65Display.h>
S65Display lcd;

#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to 
#define DHTTYPE DHT11   // DHT 11  
DHT dht(DHTPIN, DHTTYPE); 

void setup()
{
  //init LCD
  lcd.init(4); //spi-clk = Fcpu/4
  //clear screen
  lcd.clear(RGB(255,0,0));
  
}



void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();  
  
  char hum[7];   
  int hh = (h - (int)h) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(hum,"%d.%d", (int)h, abs(hh)); 
  
  char temp[7];   
  int tt = (t - (int)t) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(temp,"%d.%d", (int)t, abs(tt)); 
  
  int armcurrent = analogRead(A0); //показания аналогово выхода
  char str[5]; // объявляем текстовую строку как массив "str" из 5-ти символов
  sprintf(str, " %03i", armcurrent);
  
  int dia = 0;
  dia = map(armcurrent, 0, 1023, 1, 30);
  lcd.drawText(10, 65, str, 1, RGB( 0,255, 0), RGB(0,0,0));
  
  lcd.drawLine(10, 10, 50, 40, RGB(0,255,255));
  lcd.drawLine(10, 40, 50, 10, RGB(0,255,255));
  lcd.drawCircle(88, 30, dia, RGB(255,255,0));
  lcd.fillRect(120, 10, 150, 40, RGB(0,0,255));
  lcd.drawTextPGM(10, 50, PSTR("\x8F\xE0\xAE\xA2\xA5\xE0\xAA\xA0"), 1, RGB(0,255,0), RGB(255,0,0)); //Проверка
  lcd.drawText(10, 80, ("Temp:"), 1, RGB(255,255,  0), RGB(255,  0,  0));
  lcd.drawText(70, 80, temp, 1, RGB(255,255,  0), RGB(255,  0,  0));
  lcd.drawText(10, 95, ("Hum:"), 1, RGB(255,255,255), RGB(255,  0,  0));
  lcd.drawText(70, 95, hum, 1, RGB(255,255,255), RGB(255,  0,  0));
  lcd.drawTextPGM(10,110, PSTR(" www.watterott.com  "), 1, RGB(  0,  0,255), RGB(255,  0,  0));
 
  delay(500);
} 
