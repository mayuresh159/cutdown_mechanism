#include<EEPROM.h>
unsigned int read_bytes = 1000;

void setup()
{
  Serial.begin(115200);
  delay(5000);
}

void loop()
{
  unsigned int i;
  for (i=0;i<read_bytes;i++)
    {
      EEPROM.write(i,0);
      delay(10);
    }
   Serial.print("Done formatting");
   while(1);
}
