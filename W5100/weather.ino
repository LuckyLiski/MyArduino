// ========================Задаем необходимые библиотеки==========
#include <SPI.h>
#include <String.h>
#include <Ethernet.h>
#include "DHT.h"
#include "Wire.h"
#include "Adafruit_BMP085.h"
// ===============================================================
// ========================Задаем данные сети======================
byte mac[] = { 0xCA, 0xAF, 0x78, 0x1C, 0x13, 0x77 }; //mac - адрес ethernet shielda
byte ip[] = { 192, 168, 1, 33 };        // ip адрес ethernet shielda
byte subnet[] = { 255, 255, 255, 0 }; //маска подсети
EthernetServer server(80); //порт сервера
int ledPin = 4; // указываем что светодиод будет управляться через 4 Pin 
String readString = String(30); //string for fetching data from address
boolean LEDON = false; //изначальный статус светодиода - выключен
// ===============================================================
#define DHTPIN 3    // пин для датчика DHT22
const unsigned char OSS = 0;  // Oversampling Setting
// ===============================================================
// ========================ТИП ДАТЧИКА DHT=======================
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
// ==============================================================
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;
// ========================СТАРТУЕМ=============================
// ========================Управляем св.диодом на 4-м пине==========
void setup(){
  //запускаем Ethernet
  Ethernet.begin(mac, ip, subnet);
  //устанавливаем pin 4 на выход
  pinMode(ledPin, OUTPUT);
  //enable serial datada print
  Serial.begin(9600); 
      Serial.println("Port Test!"); // Тестовые строки для отображения в мониторе порта
      Serial.println("GO!");// Тестовые строки для отображения в мониторе порта
  dht.begin();
  bmp.begin(); 
}
// ==============================================================
void loop(){
  // ========================ДАТЧИКИ=============================
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float tdpa = bmp.readTemperature();
  float Pa0 = (bmp.readPressure());
  float Pa = (bmp.readPressure()/133.33);// переводим в мм.рт.ст.
  float Pa2 = (bmp.readPressure()/3386.582);// переводим в дм.рт.ст.
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
      Serial.print("H=: "); 
    Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temp.=: "); 
    Serial.print(t);
      Serial.println(" *C");
      Serial.print("Temp.dat.BMP = ");
    Serial.print(tdpa);
      Serial.println(" *C");
      Serial.print("Pressure = ");
    Serial.print(Pa);
      Serial.println(" mm.");    
      Serial.print("Pressure = ");
    Serial.print(Pa2);
      Serial.println(" in Hg");    
 // ==============================================================
   // =============Создаем клиентское соединение====================
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      //read char by char HTTP request
      if (readString.length() < 30) {
        //store characters to string
        readString.concat( c); }
        //output chars to serial port
        Serial.print( c);
        //if HTTP request has ended
        if (c == '\n') {
        //Проверяем включили ли светодиод?
        //Level=1 - включен
        //Level=0 - выключен
          if(readString.indexOf("Level=1") >=0) {
            //Включаем светодиод
            digitalWrite(ledPin, HIGH); // set the LED on
            LEDON = true;
          }if(readString.indexOf("Level=0") >=0) {
            //Выключаем светодиод
            digitalWrite(ledPin, LOW); // set the LED on
            LEDON = false;
          }    
   // =============Формируем HTML-страницу==========================
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<head> ");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=utf-8' /> ");
          client.println("<title> :: Упр.Arduino:: V1.1</title>");
          client.println("</head> ");
          client.println("<body");
          client.println("<hr />");
          client.println("<h1> ::Упр.Arduino:: </h1>");
          if (LEDON){
            client.println("<form method=get name=LED><input type=radio name=Level value=1 CHECKED>Включить<input type=radio name=Level value=0>Выключить<input type=submit value=OK></form>");
            client.println("<font size=’5′>LED-статус: ");
            client.println("<font size=’5′>Вкл.");
          }else{
            client.println("<form method=get name=LED><input type=radio name=Level value=1>Включить<input type=radio name=Level value=0 CHECKED>Выключить<input type=submit value=OK></form>");
            client.println("<font size=’5′>LED-статус: ");
            client.println("<font size=’5′>Выкл");
          }
         //==============Вывод значений на web-страницу======================
              client.println("<hr />");//линия=====================================
            client.println("Tемпература = ");    //Температура с DHT 22
            client.println(t);
            client.println(" *C");
              client.println("<br> "); //перенос на след. строчку
            client.println("Влажность = ");    //Влажность с DHT 22
            client.println(h);
            client.println(" %\t");
              client.println("<br> "); //перенос на след. строчку
              client.println("<hr />");//линия=====================================
            client.println("Давление = ");    //давление с BMP 085
            client.println(Pa);
            client.println(" mm.рт.ст.");
               client.println("<br> "); //перенос на след. строчку
            client.println("<hr />");  //линия=====================================
            client.println("Давление = ");    //давление с BMP 085
            client.println(Pa2);
            client.println(" in Hg");
               client.println("<br> "); //перенос на след. строчку
               client.println("<hr />");  //линия=====================================
            client.println("Tемпература внутреняя = ");//температура с BMP 085
            client.println(tdpa);
            client.println(" *C");
               client.println("<br> "); //перенос на след. строчку
               client.println("<hr />");  //линия=====================================
            //==============================================================
            client.println("</body></html>");
            //очищаем строку для следующего считывания      
            //==============Останавливаем web-client===========================
            readString="";
            client.stop();
            //==============================================================
          }
        }
      }
     }
 }
 }
