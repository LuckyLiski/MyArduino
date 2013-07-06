#include <IRremote.h>
#include <Servo.h>

int RECV_PIN = 5; //пин подключения IR приёмника 
int led_pin = 13;
Servo  servo1;
int pos1;  

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin,LOW);
  Serial.begin(9600);
  servo1.attach(3); // серво подключено к пину 3
  irrecv.enableIRIn(); // Включаем ресивер
  pos1 = 90;

}

void loop()
{
  if (irrecv.decode(&results)) 
  {
    Serial.println(results.value);
    //======================== Управление Сервой №1 ============================ 
//------------------ Уменьшаем угол открытия Сервы №1 ---------------------- 
    if (results.value == 16744575 && pos1>10) 
     { 
      delay(10);       
      pos1 = pos1-10;    
     } 
//----------------------------------------------------------------------------     

//----------------- Увеличиваем угол открытия Сервы №1 ---------------------- 
    if (results.value == 16711935 && pos1<160) 
     { 
      delay(10);       
      pos1 = pos1+10;   
     } 
//-------------------------------------------------------------------------- 
//========================================================================== 
   servo1.write(pos1);
   irrecv.resume(); // Получаем следующее значение
  }
  
  
  if (pos1 == 90) //Если угол поворота равен 90
  {digitalWrite(led_pin,HIGH);} //Включаем светодиод 13
  else
  {digitalWrite(led_pin,LOW);} //Иначе гасим светодиод 13

}


