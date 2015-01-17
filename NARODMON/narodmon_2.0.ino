// Скетч для Arduino для отправки метеоданных на Народный мониторинг.
// Версия 2.0 (19.07.2014)
//
// Автор: Гладышев Дмитрий (2012-2014)
// http://student-proger.ru/2014/07/meteostanciya-2-0/

#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <Wire.h>
#include <BMP085.h>
#include <DHT.h>

bool Debug = true; //режим отладки

//********************************************************************************************
byte mac[] = { 0xDE, 0xAD, 0xBE, 0x13, 0x04, 0x84 }; //MAC-адрес Arduino
#define BMP085_EXIST 1          // наличие датчика атмосферного давления
#define DHT_EXIST 0             // наличие датчика влажности
#define DS18B20_PIN 2           // пин подключения термодатчика DS18B20
#define DHTPIN 6                // пин подключения датчика влажности DHT22
#define DHTTYPE DHT11           // тип датчика влажности DHT22/DHT11
#define postingInterval 600000  // интервал между отправками данных в миллисекундах (10 минут)
//********************************************************************************************

IPAddress server(94,19,113,221); // IP сервера народного мониторинга
char macbuf[13];

EthernetClient client;
OneWire ds(DS18B20_PIN);

#if BMP085_EXIST == 1
  BMP085 dps = BMP085();
#endif

#if DHT_EXIST == 1
  DHT dht(DHTPIN, DHTTYPE);
#endif

unsigned long lastConnectionTime = 0;           // время последней передачи данных
boolean lastConnected = false;                  // состояние подключения
int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
char replyBuffer[160];                          // буфер для отправки
int CountSensors;                               // количество найденных датчиков температуры
long Pressure = 0;
float Humidity = 0;

void setup() {

  if (Debug)
  {
    Serial.begin(9600);
  }
  
  // секунда для инициализации Ethernet
  delay(1000);
  // Пробуем подключиться по Ethernet
  if (Ethernet.begin(mac) == 0) 
  {
    if (Debug)
    {
      Serial.println("Failed to configure Ethernet using DHCP");
    }
    // ничего не делаем
    for(;;);
  }

 //Узнаём количество термодатчиков
  CountSensors = DsCount();
  if (Debug)
  {
    Serial.print("Found ");
    Serial.print(CountSensors);
    Serial.println(" sensors."); 
  }
  
  #if BMP085_EXIST == 1
    Wire.begin();
    dps.init();
  #endif
  #if DHT_EXIST == 1
    dht.begin();
  #endif    
  
  lastConnectionTime = millis()-postingInterval+15000; //первое соединение через 15 секунд после запуска
}

void loop()
{
  //Если вдруг нам случайно приходят откуда-то какие-то данные,
  //то просто читаем их и игнорируем, чтобы очистить буфер
  if (client.available()) 
  {
    client.read();
  }

  if (!client.connected() && lastConnected) 
  {
    if (Debug)
    {
      Serial.println();
      Serial.println("disconnecting.");
    }
    client.stop();
  }

  //если не подключены и прошло определённое время, то делаем замер,
  //переподключаемся и отправляем данные
  if (!client.connected() && (millis() - lastConnectionTime > postingInterval)) 
  {

    //формирование HTTP-запроса
    memset(replyBuffer, 0, sizeof(replyBuffer));
    strcpy(replyBuffer,"ID=");

    memset(macbuf, 0, sizeof(macbuf));
    //Конвертируем MAC-адрес
    for (int k=0; k<6; k++)
    {
      int b1=mac[k]/16;
      int b2=mac[k]%16;
      char c1[2],c2[2];

      if (b1>9) c1[0]=(char)(b1-10)+'A';
      else c1[0] = (char)(b1) + '0';
      if (b2>9) c2[0]=(char)(b2-10)+'A';
      else c2[0] = (char)(b2) + '0';

      c1[1]='\0';
      c2[1]='\0';

      strcat(macbuf,c1);
      strcat(macbuf,c2);
    }
    strcat(replyBuffer, macbuf);

    //Сбрасываем поиск датчиков (кол-во нам уже известно)
    ds.reset_search();
    //Теперь в цикле опрашиваем все датчики сразу

    for (int j=0; j<CountSensors; j++)
    {

      byte i;
      byte present = 0;
      byte data[12];
      byte addr[8];

      if ( !ds.search(addr)) 
      {
        ds.reset_search();
        return;
      }

      ds.reset();
      ds.select(addr);
      ds.write(0x44,1);

      delay(1000);

      present = ds.reset();
      ds.select(addr);    
      ds.write(0xBE);

      for ( i = 0; i < 9; i++) // we need 9 bytes
      {
        data[i] = ds.read();
      }

      LowByte = data[0];
      HighByte = data[1];
      TReading = (HighByte << 8) + LowByte;
      SignBit = TReading & 0x8000;  // test most sig bit
      if (SignBit) // negative
      {
        TReading = (TReading ^ 0xffff) + 1; // 2's comp
      }
      Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

      Whole = Tc_100 / 100;  // separate off the whole and fractional portions
      Fract = Tc_100 % 100;

      char temp[3];

      itoa(Whole,temp);
      strcat(replyBuffer,"&");

      //конвертируем адрес термодатчика
      for (int k=7; k>=0; k--)
      {
        int b1=addr[k]/16;
        int b2=addr[k]%16;
        char c1[2],c2[2];

        if (b1>9) c1[0]=(char)(b1-10)+'A';
        else c1[0] = (char)(b1) + '0';
        if (b2>9) c2[0]=(char)(b2-10)+'A';
        else c2[0] = (char)(b2) + '0';

        c1[1]='\0';
        c2[1]='\0';

        strcat(replyBuffer, c1);
        strcat(replyBuffer, c2);
      }
      strcat(replyBuffer,"=");
      if (SignBit) //если температура отрицательная, добавляем знак минуса
      {
        strcat(replyBuffer,"-");
      }
      strcat(replyBuffer,temp);
      strcat(replyBuffer,".");
      if (Fract<10)
      {
        strcat(replyBuffer,"0");
      }
      itoa(Fract,temp);
      strcat(replyBuffer,temp);
    }

    char temp[8];
    long p_100, h_100;
    
    #if BMP085_EXIST == 1
      strcat(replyBuffer, "&");
      strcat(replyBuffer, macbuf);
      strcat(replyBuffer, "01=");
      dps.getPressure(&Pressure);
      p_100 = Pressure/1.333;
      Whole = p_100 / 100;
      Fract = p_100 % 100;
      itoa(Whole, temp);
      strcat(replyBuffer, temp);
      strcat(replyBuffer, ".");
      if (Fract<10)
      {
        strcat(replyBuffer,"0");
      }
      itoa(Fract, temp);
      strcat(replyBuffer, temp);
    #endif
    
    #if DHT_EXIST == 1
      Humidity = dht.readHumidity();
      strcat(replyBuffer, "&");
      strcat(replyBuffer, macbuf);
      strcat(replyBuffer, "02=");
      h_100 = Humidity*100;
      Whole = h_100 / 100;
      Fract = h_100 % 100;
      itoa(Whole, temp);
      strcat(replyBuffer, temp);
      strcat(replyBuffer, ".");
      if (Fract<10)
      {
        strcat(replyBuffer,"0");
      }
      itoa(Fract, temp);
      strcat(replyBuffer, temp);
    #endif

    strcat(replyBuffer,'\0');

    if (Debug)
    {
      Serial.println(replyBuffer);
      Serial.print("Content-Length: ");
      Serial.println(len(replyBuffer));
    }

    //отправляем запрос
    httpRequest();

  }
  //храним последнее состояние подключения
  lastConnected = client.connected();
}

void httpRequest() 
{
  if (client.connect(server, 80))
  {
    if (Debug)
    {
      Serial.println("connecting...");
    }
    // отправляем HTTP POST запрос:
    client.println("POST http://narodmon.ru/post.php HTTP/1.0");
    client.println("Host: narodmon.ru");
    //client.println("User-Agent: arduino-ethernet");
    //client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(len(replyBuffer));
    client.println();
    client.println(replyBuffer);
    client.println();

    lastConnectionTime = millis();
  } 
  else
  {
    if (Debug)
    {
      Serial.println("connection failed");
      Serial.println("disconnecting.");
    }
    client.stop();
  }
}

//Количество термодатчиков на шине
int DsCount()
{
  int count=0;
  bool thatsall = false;
  byte addr[8];
  do
  {
    if ( !ds.search(addr))
    {
      ds.reset_search();
      thatsall = true;
    }
    count++;
  } while(!thatsall);
  return (count-1);
}

int len(char *buf)
{
  int i=0; 
  do
  {
    i++;
  } while (buf[i]!='\0');
  return i;
}

void reverse(char s[])
{
  int i, j;
  char c;
  
  for (i = 0, j = strlen(s)-1; i<j; i++, j--) 
  {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int n, char s[])
{
  int i, sign;
  
  if ((sign = n) < 0)       /* записываем знак */
    n = -n;                 /* делаем n положительным числом */
  i = 0;
  do {                      /* генерируем цифры в обратном порядке */
    s[i++] = n % 10 + '0';  /* берем следующую цифру */
  } while ((n /= 10) > 0);  /* удаляем */
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}
