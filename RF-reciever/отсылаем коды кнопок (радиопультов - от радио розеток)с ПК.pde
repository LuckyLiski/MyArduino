// урок от Дмитрия Осипова. http://www.youtube.com/user/d36073?feature=watch

//отсылаем коды кнопок (радиопультов - от радио розеток)с ПК - через (монитор порта)-(COM порт)

#include <RemoteSwitch.h> // это скачанная библиотека

//коды кнопок от радиопультов, для радио розеток

// как? узнать коды кнопок пульта от радиопультов, для (радио розеток). (видеоурок я выложил на YouTube)

#define RF1ON 18062 //(18062) код кнопки от радиопульта с (8 кнопками) для радио розеток, из леруа мерлен.

#define RF1OFF 18060

#define RF2ON 19034

#define RF2OFF 19032

#define RF3ON 15146

#define RF3OFF 15144

#define RFF1 359349 //(359349)код кнопки от другого радиопульта с (3 кнопками) для радио розеток, №2.

#define RFF2 359331

#define RFF3 359323

#define PERIOD 294; //(294) period duration: - полученные из (монитор порта)

#define PERIOD 342;

#define PERIOD 343;

void setup()

{

Serial.begin(9600); //установка порта на скорость 9600 бит/сек

}

void loop()

{

while (Serial.available() == 0); //функция Serial.аvailable() проверяет, есть ли в COM порте доступные для чтения данные, мы будем «топтаться» на месте и ожидать данные.

int val = Serial.read() - '0';

if (val == 1) transmit(RF1ON); //если с COM порта поступил символ (1), начинаем отправлять через пин №11 (там радио передатчик),- код кнопки №1 радио пульта

if (val == 2) transmit(RF1OFF);

if (val == 3) transmit(RF2ON);

if (val == 4) transmit(RF2OFF);

if (val == 5) transmit(RF3ON);

if (val == 6) transmit(RF3OFF);

if (val == 7) {transmit(RF1ON);transmit(RF2ON);transmit(RF3ON);} //если с (COM порта) поступил символ (7),- (включаем сразу 3 розетки)

if (val == 8) {transmit(RF1OFF);transmit(RF2OFF);transmit(RF3OFF);}

if (val == 9) transmit(RFF1);

if (val == 0) transmit(RFF3);

}

void transmit(unsigned long rcode){

unsigned long code = rcode;

unsigned long period = PERIOD;

code |= (unsigned long)period << 23;

code |= 4L << 20; //(|= 4L) цифра перед (L), это (условное число), количества повторов посылаемого сигнала. (соответственно и паузы)

RemoteSwitch::sendTelegram(code, 11); // RF transmitter pin - пин радио передатчика

}
