/*
Пример подключения аналогового датчика газа TGS2611 (или MQ4)
Вывод на LCD в цифровом значении и прогресс бара 
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int gas = 5; // Пин датчика газа А5
float d = 0;
float oldit = 0;


void setup()
{
lcd.begin(16, 2);              // Старт дисплея
}
  
void loop()
{
  lcd.setCursor(0,0);
  lcd.print("Methane ");
  lcd.print(analogRead(gas)); // Выодим значение датчика 0-1023

  float num = analogRead(gas); // get input
  float it = map(num, 200, 1023, 0, 16);  // перевод значения аналогового входа в 16 частей для прогресс бара <== КАЛИБРОВАТЬ ЗДЕСЬ
  

//Рисуем прогресс бар
  if (it > oldit) { // если значение увеличилось
    for (d = 0; it >= d; d++) { // счетчик вверх от 0 до 15
      lcd.setCursor(d, 1); // заполняем прогресс бар
      lcd.write(1023); // закрашиваем блок
    }
  }

  if (it <= oldit) { // если значение уменьшилось
    for (d = 15; it <= d; d--) { //счетчик вниз от 0 до 15
      lcd.setCursor(d, 1); // очищаем прогресс бар
      lcd.write(1022); // стираем блок
    }
  }

  delay(500); 
  lcd.clear();
}
