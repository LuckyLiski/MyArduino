//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to 
#define DHTTYPE DHT11   // DHT 11  
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

void setup()
{
lcd.begin(16, 2);              // start the library
dht.begin(); 
Wire.begin();
RTC.begin();
//RTC.adjust(DateTime(__DATE__, __TIME__)); 
}
  
void loop()
{
DateTime now = RTC.now(); 
lcd.setCursor(0,0);            // move to the begining of the second line
lcd.print(now.hour(), DEC);
lcd.print(":");
lcd.print(now.minute(), DEC);
lcd.print(":");
lcd.print(now.second(), DEC);



float h = dht.readHumidity();
float t = dht.readTemperature();
lcd.setCursor(0,1);
lcd.print("H:");
lcd.print((int)h); //(int) - округление до целого
lcd.print("%");
lcd.print("T:");
lcd.print((int)t); //(int) - округление до целого
lcd.print("C");

}
