#include <IRremote.h>
IRsend irsend;
//Команда включения кондиционера на 25С
unsigned int powerOn[228] = {3050, 1650, 550, 1100, 550, 1150, 500, 350, 550, 350, 550, 300, 550, 1150, 500, 350, 550, 350, 550, 1100, 550, 1150, 500, 350, 550, 1150, 550, 350, 550, 300, 550, 1100, 550, 1150, 550, 300, 550, 1150, 500, 1150, 550, 350, 550, 300, 550, 1150, 500, 350, 550, 350, 550, 1150, 500, 350, 550, 300, 550, 350, 550, 350, 500, 350, 550, 350, 500, 400, 500, 350, 550, 350, 500, 350, 550, 350, 550, 350, 500, 350, 550, 300, 550, 350, 600, 300, 550, 300, 550, 1150, 550, 350, 550, 300, 550, 1100, 550, 350, 550, 350, 600, 1100, 550, 1100, 550, 350, 550, 350, 550, 300, 550, 300, 600, 300, 550, 350, 550, 300, 550, 1100, 600, 1100, 550, 350, 550, 300, 550, 350, 550, 300, 550, 350, 550, 350, 550, 300, 550, 350, 550, 350, 550, 300, 550, 300, 600, 300, 550, 350, 550, 300, 550, 350, 550, 300, 550, 350, 550, 350, 550, 300, 550, 300, 600, 350, 550, 300, 550, 300, 600, 300, 550, 350, 550, 300, 550, 350, 550, 300, 550, 350, 550, 350, 550, 300, 550, 300, 600, 350, 550, 300, 550, 300, 550, 350, 550, 350, 550, 300, 550, 350, 550, 300, 550, 350, 550, 350, 550, 300, 550, 300, 600, 350, 550, 300, 550, 1100, 550, 350, 550, 350, 550, 300, 550, 300, 550, 1150, 550, 350, 550};
void setup() {
  Serial.begin(9600);
}
void loop() {
 if(Serial.available() > 0)
   {
    int a = Serial.read();
     if(a == 49){ //Если в сериал пришла 1
       irsend.sendRaw(powerOn, 228, 36); //Команда на кондиционер
       Serial.println("Cond on");
     }
     if(a == 50){ //Если в сериал пришла 2
      irsend.sendNEC(0x20DF10EF, 32); //Кнопка Power TV LG
       Serial.println("TV on/off");
     }
     if(a == 51){ //Если в сериал пришла 3
      irsend.sendNEC(0x20DF00FF, 32); //Next channel
       Serial.println("next ch");
     }
     if(a == 52){ //Если в сериал пришла 4
      irsend.sendNEC(0x20DF48B7, 32); // кнопка 2
      irsend.sendNEC(0x20DF28D7, 32); // кнопка 4
       Serial.println("24ch");
     }
    a = 0;
   }
}
 
