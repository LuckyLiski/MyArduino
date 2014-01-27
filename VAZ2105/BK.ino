/*
ls020 pinout /распиновка дисплея сименс с65/

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

//Библиотека дисплея
#include <S65Display.h>
S65Display lcd;
//Цвета
#define BLACK        RGB(  0,  0,  0)
#define WHITE        RGB(255,255,255)
#define RED          RGB(255,  0,  0)
#define GREEN        RGB(  0,255,  0)
#define BLUE         RGB(  0,  0,255)
#define YELLOW       RGB(255,255,  0)
#define PINK         RGB(255,020,147)
#define BGCOLOR      RGB(255,246,143)

//Библиотека 1-wire и датчиков DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 8 on the Arduino
#define ONE_WIRE_BUS 8
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

