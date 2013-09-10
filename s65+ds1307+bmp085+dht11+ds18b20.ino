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

/*
Реле освещения на пин 7
Кнопка освещения на пин 3 
*/

int light = 7; //Реле освещения на пин 7
volatile int state = LOW;

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

#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to 
#define DHTTYPE DHT11   // DHT 11  
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;

#include "RTClib.h"
RTC_DS1307 RTC;

  long previousMillis = 0;
  long interval = 1000; // Интервал обновления графика
  int X=0;
  int Y=85;
  int XX=0;
  int YY=85;
  int Z=30;
  int ZZ=30;
  int W=85;
  int WW=85;
void setup()
{
// Кнопка на прерывании   
  pinMode(light, OUTPUT);              // порт как выход
  attachInterrupt(1, blink, RISING); // привязываем 1-е прерывание к функции blink(). 0 (на digital pin 2) и 1 (на digital pin 3)
/*
LOW — вызов прерывания всякий раз, когда на порту низкий уровень напряжения;
CHANGE – прерывание вызывается при изменении значения на входе;
RISING – вызов прерывания при изменении уровня напряжения с низкого (LOW) на высокое(HIGH)
FALLING – вызов прерывания при изменении уровня напряжения с высокого (HIGH) на низкое (LOW)
*/

  lcd.init(4); //spi-clk = Fcpu/4 //init LCD
  lcd.clear(BLACK); //clear screen
  lcd.fillRect(0, 0, 175, 9, BLUE); //Верхний бар
  lcd.fillRect(0, 122, 175, 131, BLUE); //Нижний бар
  lcd.fillRect(1, 11, 174, 85, GREEN); // Поле графика
  //lcd.drawLine(10, 11, 10, 85, WHITE);
  //lcd.drawLine(10, 60, 174, 60, WHITE);
  //lcd.drawText(1, 50, ("0"), 1, WHITE, GREEN);

  Wire.begin();
  RTC.begin();
  sensors.begin();
  bmp.begin();
}

void blink()
{
  static unsigned long millis_prev;
  if(millis()-millis_prev > 500) state = !state;   // меняем значение на противоположное
  millis_prev = millis();  
  digitalWrite(light, state);          // выводим state на реле 
}

void loop()
{
// Dallas  
  sensors.requestTemperatures(); // Send the command to get temperature
  float dallas = sensors.getTempCByIndex(0);
  char ds[7];   
  int dd = (dallas - (int)dallas) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(ds,"%d.%d", (int)dallas, abs(dd));

// DHT 11    
  float h = dht.readHumidity();
  float t = dht.readTemperature();  
  
  char hum[7];   
  int hh = (h - (int)h) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(hum,"%d.%d", (int)h, abs(hh)); 
  
  char temp[7];   
  int tt = (t - (int)t) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(temp,"%d.%d", (int)t, abs(tt)); 
//BMP085
  float t085 = bmp.readTemperature();  
  char temp085[7];   
  int tt085 = (t085 - (int)t085) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(temp085,"%d.%d", (int)t085, abs(tt085));  
  
  float p085 = (bmp.readPressure()*0.0075006375541921);  
  char press085[7];   
  int pp085 = (p085 - (int)p085) * 100; // множитель - число знаков после запятой. 10 - 1, 100 - 2, 1000 - 3 и т.д. 
  sprintf(press085,"%d.%d", (int)p085, abs(pp085));  
  
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
  lcd.drawText(10, 124, chasy, 1, YELLOW, BLUE); //Нижний бар
  lcd.drawText(26, 124, (":"), 1, YELLOW, BLUE);
  lcd.drawText(34, 124, minuty, 1, YELLOW, BLUE);
  lcd.drawText(50, 124, (":"), 1, YELLOW, BLUE);
  lcd.drawText(58, 124, sekundy, 1, YELLOW, BLUE);
    
  lcd.drawText(5, 2, ("DS18B20:"), 1, YELLOW, BLUE);
  lcd.drawText(100, 2, ds, 1, YELLOW, BLUE);
  lcd.drawText(140, 2, ("\xF8\x43"), 1, YELLOW, BLUE);
  
  lcd.drawText(5, 90, ("\x92\xA5\xAC\xAF\xA5\xE0\xA0\xE2\xE3\xE0\xA0:"), 1, RED, BLACK); // "Температура"
  lcd.drawText(110, 90, temp085, 1, RED, BLACK);
  lcd.drawText(150, 90, ("\xF8\x43"), 1, RED, BLACK);
  
  lcd.drawText(5, 100, ("\x82\xAB\xA0\xA6\xAD\xAE\xE1\xE2\xEC:"), 1, BLUE, BLACK); // "Влажность"
  lcd.drawText(110, 100, hum, 1, BLUE, BLACK);
  lcd.drawText(150, 100, ("%"), 1, BLUE, BLACK);
 
  lcd.drawText(5, 110, "Press:", 1, WHITE, BLACK);
  lcd.drawText(70, 110, press085, 1, WHITE, BLACK);
  lcd.drawText(130, 110, "mmHg", 1, WHITE, BLACK);

  
// График температуры
  if (millis() - previousMillis > interval) {
  previousMillis = millis();
// Температура  
  Y = map(dallas, 20, 40, 85, 11);
  X++;
  lcd.drawLine(X+1, 11, X+1, 85, RED);
  lcd.drawLine(X, 11, X, 85, GREEN);
  lcd.drawLine(XX, YY, X, Y, BLACK);
  if (X >= 175) X = 0;
  XX=X;
  YY=Y;
// Temperatura BMP085 
  W = map(t085, 20, 50, 85, 11);
  lcd.drawLine(XX, WW, X, W, RED);
  WW=W;
// Давление  
  Z = map(p085, 735, 755, 85, 11);
  lcd.drawLine(XX, ZZ, X, Z, WHITE);
  ZZ=Z;
  }
} 
