//
//    FILE: X9C_test.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo minimalistic base class for X9Cxxx devices.


#include "Arduino.h"
#include "X9C10X.h"


//  PINOUT  X9C10X TOP VIEW  (see datasheet)
//
//       +--------+
//  INC  | o    o |  VCC
//  U/D  | o    o |  CS
//  RH   | o    o |  RL
//  GND  | o    o |  Rwiper
//       +--------+
//
//  INC   pulses
//  U/D   UP = 1 DOWN = 0
//  VCC   +5V
//


//int maxJoystickLeftY=60;
//int middleJoystickLeftY=30;
//int minJoystickLeftY=0;//
//int maxJoystickLeftX=60;
//int middleJoystickLeftX=30;
//int minJoystickLeftX=0;//
//int maxTriggerLeft=30;
//int maxTriggerRight=30;
//int minTriggerLeft=0;
//int minTriggerRight=0;


X9C pot1;  //  unknown Ω  (ALT-234)
X9C pot2;  //  unknown Ω  (ALT-234)
X9C pot3;  //  unknown Ω  (ALT-234)
X9C pot4;  //  unknown Ω  (ALT-234)

int delayTime=10;


void fadeTarget(X9C pot){
 //  force to the end of the wiper.
  for (uint8_t i = 0; i < 99; i++)
  {
    pot.incr();
    Serial.print('+');
    delay(delayTime);
  }
  Serial.println();

  //  go to the wanted position.  (99 - 24 = 75)
  for (uint8_t i = 0; i < 24; i++)
  {
    pot.decr();
    Serial.print('-');
     delay(delayTime);
  }
  Serial.println();
}






void potPosition_0_99(X9C pot, int position){
 //  force to the end of the wiper.
  for (uint8_t i = 0; i < 99; i++)
  {
    pot.incr();
    Serial.print('+');
  }
  Serial.println();

  //  go to the wanted position.  (99 - 24 = 75)
  for (uint8_t i = 0; i < position; i++)
  {
    pot.decr();
    Serial.print('-');
  }
  Serial.println();

}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println();
  Serial.print("X9C10X_LIB_VERSION: ");
 // Serial.println(X9C10X_LIB_VERSION);

  pot1.begin(46, 47,10);  //  pulse, direction, select
  pot2.begin(48, 49,10);  //  pulse, direction, select
  pot3.begin(50, 51,10);  //  pulse, direction, select
  pot4.begin(52, 53,10);  //  pulse, direction, select


  //  store the current position in NV-RAM.
  //pot.store();
}


void OneAfterEachOther(){

  fadeTarget(pot1);
  fadeTarget(pot4);
  fadeTarget(pot3);
  fadeTarget(pot2);

}

void AllAtOnce(){
 //All at once
  for (uint8_t i = 0; i < 99; i++)
  {
    pot1.incr();
    pot2.incr();
    pot3.incr();
    pot4.incr();
    Serial.print('+');
  delay(delayTime);
  }
  Serial.println();

  //  go to the wanted position.  (99 - 24 = 75)
  for (uint8_t i = 0; i < 24; i++)
  {
    pot1.decr();
    pot2.decr();
    pot3.decr();
    pot4.decr();
    Serial.print('-');
  delay(delayTime);
  }
  Serial.println();
  delay(100);

}

void loop()
{

  OneAfterEachOther();
  AllAtOnce();
  //  force to the end of the wiper.
 
}


// -- END OF FILE --
