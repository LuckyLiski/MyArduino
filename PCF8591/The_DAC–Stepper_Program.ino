 #include <Wire.h>
 #define PCF8591 (0x90 >> 1)      // Device address = 0       
 #define PCF8591_DAC_ENABLE 0x40
 #define PCF8591_ADC_CH0 0x40
 #define PCF8591_ADC_CH1 0x41
 #define PCF8591_ADC_CH2 0x42
 #define PCF8591_ADC_CH3 0x43
 byte dac_value=0;
 void putDAC(byte dac_value)
 {
   Wire.beginTransmission(PCF8591);    //Calls the 8591 to attention.
   Wire.write(PCF8591_DAC_ENABLE);    //Send a DAC enable word.
   Wire.write(dac_value);            //Send the desired DAC value (0-255)
   Wire.endTransmission();
 }
 void setup()
 {
   Serial.begin(19200);    //Be sure to check the computer port's speed for this to work.
   Wire.begin();
 }
 void loop()
 {            //The IC is stepped by one notch on each loop. The green LED should slowly increase in brightness
   putDAC(dac_value);
   delay(10);
   Serial.println(dac_value);        //This goes to a terminal program
   dac_value++; 
   delay(200);
 }
