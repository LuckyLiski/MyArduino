/*
Application Name: Arduino Send SMS
Description: Send SMS messages using a Motorola C168i
Author: Jeff Murchison
Contact: jeff@jeffmurchison.com
Date: September 17, 2012
Version: 1.0
License:
 
This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 
http://creativecommons.org/licenses/by-nc-sa/3.0/
 
Attributions / Special Thanks:
 
- Matthew Sheffield. This code based on his example, but updated to use Arduino IDE 1.0.1
and SoftwareSerial.
 
http://sheffiel.blogspot.ca/2011/02/remote-car-starter-controlling-motorola.html
 
*/
 
#include <SoftwareSerial.h>
 
SoftwareSerial mySerial(3, 2); // RX, TX pins
 
void setup() {
 
pinMode(13, OUTPUT); // Initialize pin 13 as digital out (LED)
pinMode(8, INPUT); // Our button pin
mySerial.begin(4800); // Open serial connection at baud rate of 4800
 
}
 
void loop(){
 
if (digitalRead(8) == HIGH){ // On button press
digitalWrite(13, HIGH); // Turn LED on.
mySerial.println("AT"); // Sends AT command to wake up cell phone
delay(500);
mySerial.println("AT+CMGF=1"); // Puts phone into SMS mode
delay(1000); // Wait a second
mySerial.println("AT+CMGW=\"+14165551234\""); // YOUR NUMBER HERE; Creates new message to number
delay(1000);
mySerial.print("Sent from my Arduino."); // Message contents
delay(1000);
mySerial.write(byte(26)); // (signals end of message)
delay(1000);
mySerial.println("AT+CMSS=1"); // Sends message at index of 1
digitalWrite(13, LOW); // Turn LED off
delay(250);
digitalWrite(13, HIGH); // Turn LED on.
delay(10000); // Give the phone time to send the SMS
mySerial.println("AT+CMGD=1"); // Deletes message at index of 1
digitalWrite(13, LOW); // Turn LED off.
delay(250);
}
 
}
