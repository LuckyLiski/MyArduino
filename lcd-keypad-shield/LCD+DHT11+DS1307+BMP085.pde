#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;

#include "RTClib.h"
RTC_DS1307 RTC;

#include "DHT.h"
#define DHTPIN 2     // Датчик DHT на пин 2
#define DHTTYPE DHT11   // DHT 11  
DHT dht(DHTPIN, DHTTYPE);

#include <LiquidCrystal.h> //Sample using LiquidCrystal library
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // select the pins used on the LCD panel  
// Символ градуса
byte temp_cel[8] =
{
0b00111,
0b00101,
0b00111,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000
};


void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  dht.begin(); 
  lcd.begin(16, 2);              // start the library
  lcd.createChar(0, temp_cel); // Символ градуса

  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
}
  
void loop() {
 // DS1307
    lcd.setCursor(0,0); // move to the begining of the second line
   // lcd.print(now.hour(), DEC);
   // lcd.print(":");
   // lcd.print(now.minute(), DEC);
 
    DateTime now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.println();
 //BMP085
    lcd.print(bmp.readTemperature(), 1); // ", 1" - округление до 1 десятой (0 - до целых)
    lcd.print(char(0)); // Символ градуса
    lcd.print("C ");
    lcd.print(bmp.readPressure()*0.0075006375541921, 0); // ", 1" - округление до 1 десятой (0 - до целых)
    lcd.print("mmHg");
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
 
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure()*0.0075006375541921);
    Serial.println("mmHg");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
    Serial.println();
    
 //DHT11
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.print("H:");
    lcd.print((int)h); //(int) - округление до целого
    lcd.print("%");
    lcd.print("   ");
    lcd.print("T:");
    lcd.print((int)t); //(int) - округление до целого
    lcd.print("`C");

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
    Serial.println();
    Serial.println();
  }
    delay(5000);
}
