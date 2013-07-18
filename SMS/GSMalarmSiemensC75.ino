/***********************************************************
*                  SMS Alarm  
    This Software uses Arduino connected to a mobile phone(SIEMENS C75) 
    and it sends a SMS when a certain amount of light is off (LDR) and it sends 
    another (SMS) after light values are restored.
    
    Copyright (C) 2012  Carlos Oliveira  -- coli24200@gmail.com --

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***********************************************************/
/*
HARDWARE SETUP

13 -Led pin Debug 13 (used for status)
A2 -Analog input A2 (Light Sensor LDR)
2 -RX (phone) 2
3 -TX (phone) 3
 
 */
#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3


/***********************************************************
*                  PDU encoder setup       
*  Copyright (C) 2010  Daniel Richman (more in the function)
***********************************************************/
/* From hexdump.h ALIEN Project Daniel Richman */
#define num_to_char(number)   ((number) < 10 ?                           \
                                               ('0' + (number)) :        \
                                               (('A' - 10) + (number)) )
/* To select the 4 bits we do this */
#define first_four(byte)       (0x0F & (byte))

/* Last four: Shift left to get to a number < 16 */
#define  last_four(byte)      ((0xF0 & (byte)) >> 4)

/* The MSB goes first. */
#define hexdump_a(byte)  num_to_char( last_four(byte))
#define hexdump_b(byte)  num_to_char(first_four(byte))
/*
 */
//**********************************************************
// End of PDU encoder setup

//init
SoftwareSerial phone=SoftwareSerial(rxPin, txPin);
int ledPin = 13; //Used for DEBUG
int sensorPin = A2;  
int sensorValue = 0; 
int estado=0; 
//estado=3 for testing serial communication with mobile
boolean flagLUZoff=false;
boolean flagSMS=false;

/***********************************************************
*                  Setup                                   *
***********************************************************/
void setup()  
{
  pinMode(ledPin,OUTPUT); // LED DEBUG
  //Serial ------------------------------------------------- 
  // Open serial communications and wait for port to open:
  Serial.begin(9600);  // DEBUG
  while (!Serial) {;
  // wait for serial port toconnect.Needed for Leonardo only
  }

  //C75 init ----------------------------------------------
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  phone.begin(9600);

}


/***********************************************************
*                  LOOP                                    *
***********************************************************/
void loop() // run over and over
{ 
  automatoMain();
}

/***********************************************************
*                  Automato                                *
***********************************************************/

void automatoMain() {

  switch(estado) {
  case 0://check light value
    digitalWrite(ledPin, HIGH);  
    delay(1000);    
    sensorValue = analogRead(sensorPin); 
    digitalWrite(ledPin, LOW); 
    delay(1000);
    if (sensorValue < 900){
      flagLUZoff=true;
      estado=1; 
    }
    else{
      flagLUZoff=false;
      estado=2; 
    }

    break;

  case 1:// SMS send Light OFF
    delay(1000);
    if(flagLUZoff && !flagSMS){
    Serial.println("Sending SMS light is OFF"); // DEBUG
    send_sms("351969999999","Light is OFF!");
    flagSMS=true;
    }
    delay(1000);
    estado=0;
    break;

  case 2:// SMS send Light ON
    if(!flagLUZoff && flagSMS){
    Serial.println("Sending SMS light is ON"); // DEBUG
    send_sms("351969999999","Light is ON!");
    flagSMS=false;
    }   
    delay(1000);
    estado=0;
    break;

  case 3://Phone communication tests (via Serial COM)
    if (phone.available())
      Serial.write(phone.read());
    if (Serial.available())
      phone.write(Serial.read());

    break;  	

  default:
    break;
  }
}

/***********************************************************
*                  SMS PDU encript  

    This part of the code is not written by me, nevertheless I had to 
    change some parts (documented) in order to work correctly under
    Arduino V1.0. And also I added a converter function for the phone
    number, but only works for phone number with 12 digits.
    
    code was taken from Tim Zaman site: 
    (http://www.timzaman.nl/?p=47&lang=en) 
    
    **********************************************************
    
    Copyright (C) 2010  Daniel Richman (/Timbobel) (/Oliveira)
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    For a full copy of the GNU General Public License, 
    see . <http://www.gnu.org/licenses/>.
*
***********************************************************/
void hexdump_byte(unsigned char byte)
{
  phone.write(hexdump_a(byte));
  phone.write(hexdump_b(byte));
}

/***********************************************************
*                  convert Phone   (C.Oliveira)                        *
*this only works if your phone number has 12 digits, if not* 
*use the old conversion method                             *
***********************************************************/
void convertPhone(char *pho){ 
  size_t data_length,y; 
  data_length = strlen(pho);
  for (y = 0; y < data_length; y=y+2)
    {
      phone.write(pho[y+1]);
      phone.write(pho[y]);
    }
} 
//**********************************************************  

void send_sms(char *recPhone, char *data)
{
  size_t data_length, x;
  char l;
  long i;
  long n;

  data_length = strlen(data);
  i = data_length * 7;

  /* Round i up to a multiple of 8 */
  if (i & 0x07) i = (i & ~0x07) + 0x08;

  /* Calculate the number of message octets */
  i = i / 8;

  phone.println("AT+CMGF=0");
  delay(1500);
  phone.print("AT+CMGS="); // Send SMS Comand
  //phone.print("AT+CMGW=");  //Send SMS to mobile draft folder (DEBUG)
  delay(1500);
  phone.println(i + 14); //change it if necessary (see example below) Fixed number for 12 digits phone.
  delay(1500); 
  
/* Phone number is hardcoded if digits diferent from 12
   http://twit88.com/home/utility/sms-pdu-encode-decode
  use the utility to convert into pdu format, just fullfil
  the receiver number and press convert button.
  Hexadecimal PDU Message
  below the example for: 351987654321
  output:
  AT+CMGW=14  <--- size of, use above
  0011000C915391785634120000AA
  */
  // phone.print("0011000C915391785634120000AA"); // mobile: 351987654321 OLD METHOD for phones with digits !=12 
  
  phone.print("0011000C91"); // using the convertPhone function , for phones # with digits =12, like all numbers in Portugal.
  convertPhone(recPhone);
  phone.print("0000AA");

  hexdump_byte(data_length & 0xFF);

  l = 0;
  n = 0;

  for (x = 0; x < data_length; x++)
  {
    if (data[x] == '$')  data[x] = 0x02;

    n |= (data[x] & 0x7F) << l;
    l += 7;

    if (l >= 8)
    {
      hexdump_byte(n & 0xFF);
      l -= 8;
      n >>= 8;
    }
  }

  if (l != 0)
  {
    hexdump_byte(n & 0xFF);
  }
  // Code changed for Arduino v1.0
  phone.write(0x1A); // Ctrl+Z
  phone.write(0x0D); // CR
  phone.write(0x0A); // LF
  delay(5000);
  
}
