#include <S65Display.h>
S65Display lcd;

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 8 on the Arduino
#define ONE_WIRE_BUS 8
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to 
#define DHTTYPE DHT11   // DHT 11  
DHT dht(DHTPIN, DHTTYPE); 

void setup()
{
  lcd.init(4); //spi-clk = Fcpu/4 //init LCD
  lcd.clear(RGB(255,255,255)); //clear screen
  lcd.fillRect(0, 0, 175, 12, RGB(0,0,255)); //Верхний бар
  lcd.drawText(0, 3, ("   MeteoStation v1.0"), 1, RGB(0,255,120), RGB(0,0,255)); //Верхний бар
  lcd.fillRect(0, 120, 175, 131, RGB(0,0,255)); //Нижний бар
  
  sensors.begin();
}



void loop()
{
  sensors.requestTemperatures(); // Send the command to get temperature
  float dallas = sensors.getTempCByIndex(0);
  char ds[7];   
  int dd = (dallas - (int)dallas) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(ds,"%d.%d", (int)dallas, abs(dd));
  lcd.drawText(10, 35, ("DS18B20:"), 1, RGB(255,0,0), RGB(255,255,255));
  lcd.drawText(100, 35, ds, 1, RGB(255,0,0), RGB(255,255,255));
  lcd.drawText(140, 35, ("\xF8\x43"), 1, RGB(255,0,0), RGB(255,255,255));
    
  float h = dht.readHumidity();
  float t = dht.readTemperature();  
  
  char hum[7];   
  int hh = (h - (int)h) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(hum,"%d.%d", (int)h, abs(hh)); 
  
  char temp[7];   
  int tt = (t - (int)t) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(temp,"%d.%d", (int)t, abs(tt)); 
  
  int armcurrent = analogRead(A0); //показания аналогово входа
  armcurrent = map(armcurrent, 0, 1023, 0, 99);
  char str[5]; // объявляем текстовую строку как массив "str" из 5-ти символов
  sprintf(str, " %02i", armcurrent);
  
  lcd.drawText(10, 111, "Poten:", 1, RGB( 255,0, 0), RGB(255,255,255));
  lcd.drawText(59, 111, str, 1, RGB( 255,0, 0), RGB(255,255,255));
  lcd.drawText(83, 111, "%", 1, RGB( 255,0, 0), RGB(255,255,255));
  
  //lcd.drawLine(10, 10, 50, 40, RGB(0,255,255));
  //lcd.drawLine(10, 40, 50, 10, RGB(0,255,255));
  //lcd.drawCircle(88, 30, dia, RGB(255,255,0));
  //lcd.fillRect(120, 10, 150, 40, RGB(0,0,255));
  
  lcd.drawText(10, 123, ("Menu     <-       ->"), 1, RGB(255,255,255), RGB(0,0,255)); //Нижний бар
  
  //lcd.drawTextPGM(10, 50, PSTR("\x8F\xE0\xAE\xA2\xA5\xE0\xAA\xA0"), 1, RGB(0,255,0), RGB(255,0,0)); //Проверка
  lcd.drawText(10, 15, ("Temp:"), 1, RGB(255,0,0), RGB(255,255,255));
  lcd.drawText(100, 15, temp, 1, RGB(255,0,0), RGB(255,255,255));
  lcd.drawText(140, 15, ("\xF8\x43"), 1, RGB(255,0,0), RGB(255,255,255));
  
  lcd.drawText(10, 25, ("Hum:"), 1, RGB(0,0,255), RGB(255,255,255));
  lcd.drawText(100, 25, hum, 1, RGB(0,0,255), RGB(255,255,255));
  lcd.drawText(140, 25, ("%"), 1, RGB(255,0,0), RGB(255,255,255));
  //lcd.drawTextPGM(10,110, PSTR(" www.watterott.com  "), 1, RGB(  0,  0,255), RGB(255,  0,  0));
 
  //delay(500);
} 
