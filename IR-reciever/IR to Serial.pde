/*************************************************************
project: IR to Serial
author: Gladyshev Dmitriy
description: Выводит в консоль код нажатой кнопки пульта
IDE: Arduino 1.0.1
*************************************************************/

#include <IRremote.h>

int RECV_PIN = 3; //пин подключения IR приёмника

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  pinMode(7,OUTPUT);
  irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
  if (irrecv.decode(&results)) 
  {
    Serial.println(results.value);
    
    irrecv.resume(); // Receive the next value
  }
}
