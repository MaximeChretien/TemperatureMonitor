/*
DS18B20 temperature sensor and 4 7digits diplay control 

Maxime Chretien

v1.2
*/


#include <OneWire.h>

const int digitsPins[8] = {5, 4, 2, 1, 0, 7, 8, 3};
const int displaysPins[4] = {6, 9, 10, 11};
const int dallasPin = 12;

const unsigned long delayMillis = 1;

const bool zero[7] = {LOW, LOW, LOW, LOW, LOW, LOW, HIGH};
const bool one[7] = {HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH};
const bool two[7] = {LOW, LOW, HIGH, LOW, LOW, HIGH, LOW};
const bool three[7] = {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW};
const bool four[7] = {HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW};
const bool five[7] = {LOW, HIGH, LOW, LOW, HIGH, LOW, LOW};
const bool six[7] = {LOW, HIGH, LOW, LOW, LOW, LOW, LOW};
const bool seven[7] = {LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH};
const bool eight[7] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW};
const bool nine[7] = {LOW, LOW, LOW, LOW, HIGH, LOW, LOW};
const bool minus[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW};


float temp;
int tempTab[3] = {0, 0, 0};
unsigned long oldMillis;

void setup() {
  for(int i=0; i<8; i++)  {
    pinMode(digitsPins[i], OUTPUT);
    digitalWrite(digitsPins[i], HIGH);
  }

  for(int i=0; i<4; i++)  {
    pinMode(displaysPins[i], OUTPUT);
    digitalWrite(displaysPins[i], LOW);
  }

  getDallasTemp(dallasPin, 1);
  oldMillis = millis();
}

void loop() {
  allDigitsOff();
  
  temp = getDallasTemp(dallasPin, 0);
  
  tempTab[0] = (int)temp / 10;
  tempTab[1] = (int)temp % 10;
  tempTab[2] = (int)(temp * 10) % 10;

  oldMillis = millis();
  
  allDigitsOff();
  allDisplaysOff();
  
  showDP(false);
  if(temp > 0)
  	showMinus();
  else
  	allDigitsOff();
  selectDisplay(0);
   
  while(millis()-oldMillis<delayMillis) {};
  oldMillis = millis();

  allDigitsOff();
  allDisplaysOff();

  show(tempTab[0]);
  showDP(false);
  selectDisplay(1);
  
  while(millis()-oldMillis<delayMillis) {};
  oldMillis = millis();

  allDigitsOff();
  allDisplaysOff();

  showDP(true);
  show(tempTab[1]);
  selectDisplay(2);
  
  while(millis()-oldMillis<delayMillis) {};
  oldMillis = millis();

  allDigitsOff();
  allDisplaysOff();
  
  showDP(false);
  show(tempTab[2]);
  selectDisplay(3);

  while(millis()-oldMillis<delayMillis) {};
  
}

float getDallasTemp(int pin, byte start) {
   OneWire ds(pin);
   byte i;
   byte data[12];
   int16_t result;
   float celsius;
   byte type_s = 0;

   do {
      ds.reset();
      ds.write(0xCC);
      ds.write(0xBE);
  
      for(i=0; i<9; i++) data[i] = ds.read();
  
      result=(data[1]<<8) | data[0];
      if (type_s) {
        result = result << 3; // 9 bit resolution default
        if (data[7] == 0x10) {
          // "count remain" gives full 12 bit resolution
          result = (result & 0xFFF0) + 12 - data[6];
        }
      } else {
        byte cfg = (data[4] & 0x60);
        // at lower res, the low bits are undefined, so let's zero them
        if (cfg == 0x00) result = result & ~7;  // 9 bit resolution, 93.75 ms
        else if (cfg == 0x20) result = result & ~3; // 10 bit res, 187.5 ms
        else if (cfg == 0x40) result = result & ~1; // 11 bit res, 375 ms
        //// default is 12 bit resolution, 750 ms conversion time
      }
      celsius = (float)result / 16.0;
      //result>>=4; if (data[1]&128) result|=61440;
  
      //if (data[0]&8) ++result;

      ds.reset();
      ds.write(0xCC);
      ds.write(0x44,1);
    
      if (start) delay(1000);
    
   } while (start--);
   
   return celsius;
}

void selectDisplay(int disp)  {
  allDisplaysOff();
  digitalWrite(displaysPins[disp], HIGH);
}

void allDisplaysOff()  {
  for(int i=0; i<4; i++)  {
    digitalWrite(displaysPins[i], LOW);
  }
}

void allDigitsOff()
{
  for(int i=0; i<8; i++)  {
    digitalWrite(digitsPins[i], HIGH);
  }
}

void show(int nb)
{
  switch (nb)
  {
    case 0:
      show0();
      break;
    case 1:
      show1();
      break;
    case 2:
      show2();
      break;
    case 3:
      show3();
      break;
    case 4:
      show4();
      break;
    case 5:
      show5();
      break;
    case 6:
      show6();
      break;
    case 7:
      show7();
      break;
    case 8:
      show8();
      break;
    case 9:
      show9();
      break;
  }
}

void show0()
{
  for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], zero[i]);
  }
}

void show1()
{
  for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], one[i]);
  }
}

void show2()
{
  for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], two[i]);
  }
}

void show3()
{
  for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], three[i]);
  }
}

void show4()
{
  for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], four[i]);
  }
}

void show5()
{
  for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], five[i]);
  }
}

void show6()
{
  for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], six[i]);
  }
}

void show7()
{
  for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], seven[i]);
  }
}

void show8()
{
  for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], eight[i]);
  }
}

void show9()
{
  for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], nine[i]);
  }
}

void showMinus() {
	for(int i=0; i<7; i++)  {
    digitalWrite(digitsPins[i], minus[i]);
  }
}

void showDP(bool dp)
{
  if (dp)
    digitalWrite(digitsPins[7], LOW);
  else
    digitalWrite(digitsPins[7], HIGH);
}
