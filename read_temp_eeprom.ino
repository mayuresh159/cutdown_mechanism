#include<EEPROM.h>
unsigned int read_bytes = 1000;

void setup()
{
  Serial.begin(115200);
  delay(5000);
}

void loop()
{
  unsigned int i, val;
  for (i=0;i<read_bytes;i++)
    {
      val = EEPROM.read(i);
      delay(5);
      Serial.println(val,DEC);
      delay(100);
    }
    while(1);
}
