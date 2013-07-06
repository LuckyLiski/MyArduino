#include <Servo.h>
int val;
Servo  servo;
void setup()
{
  servo.attach(3); // серво подключено к пину 3
  Serial.begin(9600); // скорость поледовательного порта
}
 
void loop() { 
  if(Serial.available()>0) {
  val = Serial.read(); //читаем данные
  //servo.write(Buffer);  //и поворачиваем серву на нужный угол        
  // delay(15); 
  if (val=='1') servo.write(20);
  if (val=='2') servo.write(40);
  if (val=='3') servo.write(60);
  if (val=='4') servo.write(80);
  if (val=='5') servo.write(100);
  if (val=='6') servo.write(120);
  if (val=='7') servo.write(140);
  if (val=='8') servo.write(160);
  if (val=='9') servo.write(180);
  }
}
