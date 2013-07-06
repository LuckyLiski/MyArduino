/*************************************************************
project: IR Light v1.0
author: Gladyshev Dmitriy
description: Управление освещением с IR ПДУ
IDE: 1.0.1
2 марта 2013
*************************************************************/

#include <IRremote.h>

int RECV_PIN = 3; //пин подключения IR приёмника
int RELAY_PIN = 7; //пин подключения реле

IRrecv irrecv(RECV_PIN);
decode_results results;
bool LampState = false;

void setup()
{
  pinMode(RELAY_PIN,OUTPUT);
  digitalWrite(RELAY_PIN,LOW);
  irrecv.enableIRIn(); // Включаем ресивер
}

void loop()
{
  if (irrecv.decode(&results)) 
  {
    if (results.value == 16744575) //Код кнопки
	  {
		  LampState = !LampState;
		  if (LampState)
		  {
			  digitalWrite(RELAY_PIN,HIGH);
		  }
		  else
		  {
			  digitalWrite(RELAY_PIN,LOW);
		  }
	  }
    irrecv.resume(); // Получаем следующее значение
  }
}
