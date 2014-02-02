/*
ls020 pinout

LED-          GND
LED+          +9v
1V8           n/c
GND           GND
2V9           +3,3v
DAT   MOSI    pin 11
CLK   SCK     pin 13
CS            pin A2
RESET         pin A3
RS            pin 4
*/

#include <S65Display.h>
S65Display lcd;

#define BLACK        RGB(  0,  0,  0)
#define WHITE        RGB(255,255,255)
#define RED          RGB(255,  0,  0)
#define GREEN        RGB(  0,255,  0)
#define BLUE         RGB(  0,  0,255)
#define YELLOW       RGB(255,255,  0)
#define PINK         RGB(255,020,147)
#define BGCOLOR      RGB(255,246,143)

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 8 on the Arduino
#define ONE_WIRE_BUS 8
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

int ax= 20;
int ay= 30;

void setup()
{
  lcd.init(4); //spi-clk = Fcpu/4 //init LCD
  lcd.clear(WHITE); //clear screen
  //lcd.fillRect(0, 0, 175, 9, BLUE); //Верхний бар
  //lcd.fillRect(0, 122, 175, 131, BLUE); //Нижний бар
  //lcd.fillRect(1, 11, 174, 85, GREEN); // Поле графика
  //lcd.drawLine(10, 11, 10, 85, WHITE);
  //lcd.drawLine(10, 60, 174, 60, WHITE);
  //lcd.drawText(1, 50, ("0"), 1, WHITE, GREEN);

  Wire.begin();
  RTC.begin();
  sensors.begin();
}

void loop()
{
// Dallas  
  sensors.requestTemperatures(); // Send the command to get temperature
  float dallas = sensors.getTempCByIndex(0);
  char ds[7];   
  int dd = (dallas - (int)dallas) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(ds,"%d.%d", (int)dallas, abs(dd));

// Часы
  DateTime now = RTC.now(); 
  int hr = now.hour();  
  char chasy[2];   
  int hrhr = (hr - (int)hr); // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(chasy,"%d", (int)hr, abs(hrhr)); 
  int m = now.minute();  
  char minuty[2];   
  int mm = (m - (int)m); // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(minuty,"%d", (int)m, abs(mm)); 
  int s = now.second();  
  char sekundy[2];   
  int ss = (s - (int)s); // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(sekundy,"%d", (int)s, abs(ss)); 

// Рисуем  
/*
  lcd.drawText(10, 124, chasy, 1, YELLOW, BLUE); //Нижний бар
  lcd.drawText(26, 124, (":"), 1, YELLOW, BLUE);
  lcd.drawText(34, 124, minuty, 1, YELLOW, BLUE);
  lcd.drawText(50, 124, (":"), 1, YELLOW, BLUE);
  lcd.drawText(58, 124, sekundy, 1, YELLOW, BLUE);
*/    

  lcd.drawText(33, 2, ds, 1, RED,WHITE);
  lcd.drawText(95, 2, ("\xF8\x43"), 1, RED, WHITE);
  
  
  
  //Значок аккумулятора
  lcd.drawRect(ax, ay, ax+30, ay+20, RED);
  lcd.fillRect(ax+5, ay, ax+10, ay-3, RED);
  lcd.fillRect(ax+20, ay, ax+25, ay-3, RED);
  lcd.drawLine(ax+5, ay+5, ax+10, ay+5, RED);
  lcd.drawLine(ax+20, ay+5, ax+26, ay+5, RED);
  lcd.drawLine(ax+23, ay+2, ax+23, ay+8, RED);
  
  // Значок тормоза
  lcd.drawText(70, 55, ("\x28\x21\x29"), 1, RED, WHITE); // (!)
  lcd.drawCircle(87, 61, 10, RED);

  
    // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (20.0 / 1023.0);
  char volt[7];   
  int vv = (voltage - (int)voltage) * 10; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(volt,"%d.%d", (int)voltage, abs(vv));
  lcd.drawText(ax+33, ay+2, volt, 1, RED, WHITE);
  lcd.drawText(ax+95, ay+2, ("VOLT"), 1, RED, WHITE);
} 
