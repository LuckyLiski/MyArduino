//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to 
#define DHTTYPE DHT11   // DHT 11  
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
lcd.begin(16, 2);              // start the library
dht.begin(); 
}
  
void loop()
{

float h = dht.readHumidity();
float t = dht.readTemperature();

lcd.setCursor(0,0);            // move to the begining of the second line
lcd.print("Humidity: ");
lcd.print(h);
lcd.print("%");
lcd.setCursor(0,1);
lcd.print("Temperat: ");
lcd.print(t);
lcd.print("C");

}
