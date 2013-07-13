#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;

#include "DHT.h"
#define DHTPIN 2     // Датчик DHT на пин 2
#define DHTTYPE DHT11   // DHT 11  
DHT dht(DHTPIN, DHTTYPE);

#include <DS1307.h> // written by  mattt on the Arduino forum and modified by D. Sjunnesson
char buffer[9];  //для вывода времени и даты на лсд

#include <LiquidCrystal.h> //Sample using LiquidCrystal library
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // select the pins used on the LCD panel  

void setup() {
  
  Serial.begin(9600);
  Wire.begin();
  dht.begin(); 
  lcd.begin(16, 2);              // start the library

  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
}
  
void loop() {

    lcd.setCursor(0,0); // move to the begining of the second line
 
 //BMP085
    lcd.print(bmp.readTemperature(), 1); // ", 1" - округление до 1 десятой (0 - до целых)
    lcd.write(223); // Символ градус
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
    lcd.print(" ");
//    lcd.print("T:");
//    lcd.print((int)t); //(int) - округление до целого
//    lcd.write(223); // Символ градус
//    lcd.print("C");

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
  
 // DS1307
  
//"%" означает, что дальше идут символы форматирования
//"02" - спецификатор ширины поля (2 символа), 0 указывает на то, что пустые позиции заполняются нулями.
//"d" - вывод в формате целого числа
//"-" - все, что стоит после спецификатора типа до следующего знака процента поступает на выход без изменения.
   
  sprintf(buffer,"%02d:%02d:%02d",RTC.get(DS1307_HR,true),RTC.get(DS1307_MIN,false),RTC.get(DS1307_SEC,false));
  lcd.print(buffer);
 // sprintf(buffer,"%02d-%02d-%02d",RTC.get(DS1307_DATE,false),RTC.get(DS1307_MTH,false),RTC.get(DS1307_YR,false));
 // lcd.print(buffer);
   
  Serial.print(RTC.get(DS1307_HR,true)); //read the hour and also update all the values by pushing in true
  Serial.print(":");
  Serial.print(RTC.get(DS1307_MIN,false));//read minutes without update (false)
  Serial.print(":");
  Serial.print(RTC.get(DS1307_SEC,false));//read seconds
  Serial.print("      ");                 // some space for a more happy life
  Serial.print(RTC.get(DS1307_DATE,false));//read date
  Serial.print("/");
  Serial.print(RTC.get(DS1307_MTH,false));//read month
  Serial.print("/");
  Serial.print(RTC.get(DS1307_YR,false)); //read year
  Serial.println();
  
    delay(1000);
}
