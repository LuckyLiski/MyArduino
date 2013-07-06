/* урок от Дмитрия Осипова - узнаём коды кнопок пульта от радиопультов, для (радио розеток)

http://www.youtube.com/user/d36073?feature=watch*/

#include <RemoteReceiver.h>

// Connect the receiver to digital pin 2.-пин 2.это вход для Радиоприёмника

void setup() {

Serial.begin(9600);

RemoteReceiver::init(0, 3, showCode);

}

void loop() {

}

void showCode(unsigned long receivedCode, unsigned int period) {

Serial.print("Code: ");

Serial.print(receivedCode);

Serial.print(", period duration: ");

Serial.print(period);

Serial.println("us.");

}

/*жмем на иконку с лупой, в верхнем правом углу (монитор порта)

записываем полученные коды (можно на бумаге) шутка!*/
