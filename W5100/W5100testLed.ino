#include <SPI.h>
#include <String.h>
#include <Ethernet.h>
/*Управление светодиодом через Ethernet */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //mac - адрес ethernet shielda
byte ip[] = { 192, 168, 1, 55 };        // ip адрес ethernet shielda
byte subnet[] = { 255, 255, 255, 0 }; //маска подсети
EthernetServer server(80); //порт сервера
int ledPin = 4; // указываем что светодиод будет управляться через 4 Pin 
String readString = String(30); //string for fetching data from address
boolean LEDON = false; //изначальный статус светодиода - выключен

void setup(){
  //запускаем Ethernet
  Ethernet.begin(mac, ip, subnet);
  //устанавливаем pin 4 на выход
  pinMode(ledPin, OUTPUT);
  //enable serial datada print
  Serial.begin(9600); 
}
void loop(){
  // Создаем клиентское соединение
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
          }
          if(readString.indexOf("Level=0") >=0) {
            //Выключаем светодиод
            digitalWrite(ledPin, LOW); // set the LED on
            LEDON = false;
          }
          // Формируем HTML
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<head> ");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=utf-8' /> ");
          client.println("<title> Arduino :: Управление V0.1</title>");
          client.println("</head> ");
          client.println("<body");
          //controlling led via radio elements
          client.println("<h1>Управление светодиодом 4pin</h1>");
          if (LEDON){
            client.println("<form method=get name=LED><input type=radio name=Level value=1 CHECKED>Включить<input type=radio name=Level value=0>Выключить<input type=submit value=Применить></form>");
            client.println("<font size=’5′>LED-статус: ");
            client.println("<font size=’5′>Вкл.");
          }else{
            client.println("<form method=get name=LED><input type=radio name=Level value=1>Включить<input type=radio name=Level value=0 CHECKED>Выключить<input type=submit value=Применить></form>");
            client.println("<font size=’5′>LED-статус: ");
            client.println("<font size=’5′>Выкл");
          }
            client.println("<hr />");
            client.println("</body></html>");
            //clearing string for next read
            readString="";
            //останавливаем web-client
            client.stop();
          }
        }
      }
    }
  }
