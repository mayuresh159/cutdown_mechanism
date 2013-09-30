//Arduino 1.0+ Only
//Arduino 1.0+ Only

//////////////////////////////////////////////////////////////////
//©2011 bildr
//Released under the MIT License - Please reuse change and share
//Simple code for the TMP102, simply prints temperature via serial
//////////////////////////////////////////////////////////////////

#include <Wire.h>
int lm75Address = 0x48;

int celsius;

void setup(){
  Serial.begin(115200);
  Wire.begin();
}

void loop(){

  getTemperature();
  Serial.print("Celsius: ");
  Serial.println(celsius);

  delay(2000); //just here to slow down the output. You can remove this
}



void getTemperature(){ 
  
  Wire.requestFrom(0x48,2);
  byte MSB = Wire.read();
  byte LSB = Wire.read();

  //it's a 12bit int, using two's compliment for negative
  //int TemperatureSum = ((MSB << 8) | LSB) >> 4; 
  signed int TemperatureSum = MSB;
  
  celsius = TemperatureSum;

}
