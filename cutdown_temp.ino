#include <EEPROM.h>
#include <Wire.h>

// EEPROM addresses
// addr value
//  0    LSB
//  1    MSB
//  2    FLAG
//  3    addr LSB
//  4    addr MSB
//  5    temp data starts

// macros
unsigned int cutdown_thresh = 40*60;
unsigned int on_time = 30;
unsigned int inc_time = 100;
int lm75Address = 0x48;
unsigned int count_lsb = 0, count_msb = 1, cutdown_flag = 2, addr_lsb = 3, addr_msb = 4, data_addr = 5;

// pin placements
int relay_pin = 2;
int switch_pin = 3;
int led_pin = 13;

// global variables
unsigned int count;
unsigned int min_count=0;
unsigned int addr = data_addr;

void setup()
{
 pinMode(relay_pin,OUTPUT); digitalWrite(relay_pin,LOW);
 pinMode(switch_pin,INPUT); digitalWrite(switch_pin,HIGH);
 pinMode(led_pin,OUTPUT); digitalWrite(led_pin,LOW);
 attachInterrupt(1,startcount,FALLING);
 Serial.begin(115200);
 Wire.begin();
 EEPROM.write(cutdown_flag,0);
 delay(5000);
}

void loop()
{
  min_count++;
  readcount();
  count = count + 1;
  Serial.println(count,DEC);
  writecount();
  // timing for the cutdown mechanism
  if((count > cutdown_thresh) && (count < (cutdown_thresh + on_time))) {digitalWrite(relay_pin,HIGH); EEPROM.write(cutdown_flag,1); delay(5);}
  if(count >= cutdown_thresh+on_time) {digitalWrite(relay_pin,LOW);}
  // timing for the temperature value
  if (min_count>=30) {recordtemp(); min_count = 0;}
  // indicator to define the working of the board
  digitalWrite(led_pin,!digitalRead(led_pin));
  delay (inc_time-40);
}

void startcount()
{
  char i=0;
  delay(10);
  i = digitalRead(switch_pin);
  if (i==0)
  {EEPROM.write(count_lsb,0);
  delay(10);
  EEPROM.write(count_msb,0);
  delay(10);
  EEPROM.write(cutdown_flag,0);
  addr = data_addr;
  writeaddr();
  min_count=0;}

}


void readcount()
{
  unsigned char countl,counth;
  countl = EEPROM.read(count_lsb);
  delay(10);
  counth = EEPROM.read(count_msb);
  delay(10);
  count = ((int)counth*256)+ (int)countl;
}

void writecount()
{
  unsigned char countl,counth;
  countl = count%256;
  counth = count>>8;
  EEPROM.write(count_lsb,countl);
  delay(10);
  EEPROM.write(count_msb,counth);
  delay(10);
}


void recordtemp(){ 
  Wire.requestFrom(lm75Address,2);
  //delay(5);
  byte MSB = Wire.read();
  byte LSB = Wire.read();
  signed int Temperature = MSB;
  Serial.print("Temperature : "); Serial.print(Temperature,DEC); Serial.print("\t");
  readaddr();
  if (addr<3*(cutdown_thresh/30)+data_addr)
    {
     EEPROM.write(addr,Temperature);
     delay(5);
     addr++;
    }
  Serial.print("Address : "); Serial.println(addr,DEC);
  writeaddr();
}

void readaddr()
{
  unsigned char addrl,addrm;
  addrl = EEPROM.read(addr_lsb);
  delay(10);
  addrm = EEPROM.read(addr_msb);
  delay(10);
  addr = ((int)addrm*256)+ (int)addrl;
}

void writeaddr()
{
  unsigned char addrl, addrm;
  addrl = addr%256;
  addrm = addr>>8;
  EEPROM.write(addr_lsb,addrl);
  delay(10);
  EEPROM.write(addr_msb,addrm);
  delay(10);
}
