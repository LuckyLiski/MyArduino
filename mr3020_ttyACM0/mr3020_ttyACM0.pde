#define ledPin 13  
void setup()  
{   
  Serial.begin(57600);   
  pinMode(ledPin, OUTPUT);       
}  

void loop()  
{  
  if (Serial.available() > 0)  
  {  
    byte inByte = Serial.read();  
    switch (inByte)  
    {  
        case 49:  
        digitalWrite(ledPin, HIGH);  
        Serial.println("ON!");  
        break;  
        
        case 48:  
        digitalWrite(ledPin, LOW); 
        Serial.println("OFF");  
        break;  
    }          
  }  
}
