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

  long previousMillis = 0;
  long interval = 1000; // Интервал обновления графика
  int X=0;
  int Y=85;
  int XX=0;
  int YY=85;
  
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
  lcd.fillRect(1, 11, 174, 85, GREEN);
  
  sensors.begin();
  
}

void blink()
{
  state = !state;                    // меняем значение на противоположное
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
  
// Potentiometer  
  int armcurrent = analogRead(A0); //показания аналогово входа
  armcurrent = map(armcurrent, 0, 1023, 0, 99);
  char str[5]; // объявляем текстовую строку как массив "str" из 5-ти символов
  sprintf(str, " %02i", armcurrent);

// Рисуем  
  //lcd.drawText(0, 3, ("   MeteoStation v1.0"), 1, YELLOW, BLUE); //Верхний бар
  lcd.drawText(5, 124, ("6/09/2013       11:53"), 1, YELLOW, BLUE); //Нижний бар
  
  //lcd.drawTextPGM(10, 50, PSTR("\x8F\xE0\xAE\xA2\xA5\xE0\xAA\xA0"), 1, RGB(0,255,0), RGB(255,0,0)); //Проверка
    
  lcd.drawText(5, 2, ("DS18B20:"), 1, YELLOW, BLUE);
  lcd.drawText(100, 2, ds, 1, YELLOW, BLUE);
  lcd.drawText(140, 2, ("\xF8\x43"), 1, YELLOW, BLUE);
  
  lcd.drawText(5, 90, ("Temperature:"), 1, RED, BLACK);
  lcd.drawText(110, 90, temp, 1, RED, BLACK);
  lcd.drawText(150, 90, ("\xF8\x43"), 1, RED, BLACK);
  
  lcd.drawText(5, 100, ("Humidity:"), 1, BLUE, BLACK);
  lcd.drawText(110, 100, hum, 1, BLUE, BLACK);
  lcd.drawText(150, 100, ("%"), 1, BLUE, BLACK);
  //lcd.drawTextPGM(10,110, PSTR(" www.watterott.com  "), 1, RGB(  0,  0,255), RGB(255,  0,  0));
 
  lcd.drawText(5, 110, "Potentiomet:", 1, WHITE, BLACK);
  lcd.drawText(102, 110, str, 1, WHITE, BLACK);
  lcd.drawText(150, 110, "%", 1, WHITE, BLACK);
  
  //lcd.drawLine(10, 10, 50, 40, RGB(0,255,255));
  //lcd.drawLine(10, 40, 50, 10, RGB(0,255,255));
  //lcd.drawCircle(88, 30, dia, RGB(255,255,0));
  //lcd.fillRect(120, 10, 150, 40, RGB(0,0,255));
  

// График температуры
  if (millis() - previousMillis > interval) {
  previousMillis = millis();
  Y = map(dallas, 20, 60, 85, 11);
  X++;
  lcd.drawLine(X+1, 11, X+1, 85, RED);
  lcd.drawLine(X, 11, X, 85, GREEN);
  lcd.drawLine(XX, YY, X, Y, BLACK);
  if (X >= 175) X = 0;
  XX=X;
  YY=Y;
  }
} 
