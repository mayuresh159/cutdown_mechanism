#include <EEPROM.h>

// macros
unsigned int cutdown_thresh = 10;
unsigned int on_time = 30;
unsigned int inc_time = 1000;

// pin placements
int relay_pin = 2;
int switch_pin = 3;
int led_pin = 13;

// global variables
unsigned int count;

void setup()
{
 pinMode(relay_pin,OUTPUT); digitalWrite(relay_pin,LOW);
 pinMode(switch_pin,INPUT); digitalWrite(switch_pin,HIGH);
 pinMode(led_pin,OUTPUT); digitalWrite(led_pin,LOW);
 attachInterrupt(1,start_count,FALLING);
 Serial.begin(115200);
 delay(1000);
}

void loop()
{
  read_eeprom();
  count = count + 1;
  Serial.println(count,DEC);
  write_eeprom();
  if(count == cutdown_thresh) {digitalWrite(relay_pin,HIGH);}
  if(count == cutdown_thresh+on_time) {digitalWrite(relay_pin,LOW);}
  digitalWrite(led_pin,!digitalRead(led_pin));
  delay (inc_time-40);
}

void start_count()
{
  char i=0;
  delay(10);
  i = digitalRead(switch_pin);
  if (i==0)
  {EEPROM.write(0,0);
  delay(10);
  EEPROM.write(1,0);
  delay(10);}

}


void read_eeprom()
{
  int countl,counth;
  countl = EEPROM.read(0);
  delay(10);
  counth = EEPROM.read(1);
  delay(10);
  count = ((int)counth*256)+ (int)countl;
}

void write_eeprom()
{
  unsigned char countl,counth;
  countl = count%256;
  counth = count>>8;
  EEPROM.write(0,countl);
  delay(10);
  EEPROM.write(1,counth);
  delay(10);
}
