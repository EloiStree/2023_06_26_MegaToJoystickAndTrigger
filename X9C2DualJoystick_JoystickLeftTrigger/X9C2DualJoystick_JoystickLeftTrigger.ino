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


int relayPinCount=16;
int relayPin [] = {22,24,26,28,30,32,34,36,   23,25,27,29,31,33,35,37};



int maxJoystickY = 50;
int middleJoystickY = 25;
int minJoystickY = 0;  //

int maxJoystickX = 50;
int middleJoystickX = 25;
int minJoystickX = 0;  //



int maxTriggerLeft = 50;
int maxTriggerRight = 50;
int minTriggerLeft = 0;
int minTriggerRight = 0;


X9C potLeftTrigger;     //  unknown Ω  (ALT-234)
X9C potRightTrigger;    //  unknown Ω  (ALT-234)
X9C potLeftHorizontal;  //  unknown Ω  (ALT-234)
X9C potLeftVertical;    //  unknown Ω  (ALT-234)
X9C potRightHorizontal;  //  unknown Ω  (ALT-234)
X9C potRightVertical;    //  unknown Ω  (ALT-234)

int delayTime = 10;



void initRelay(){
 for (uint8_t i = 0; i < relayPinCount; i++) {
      pinMode(relayPin[i], OUTPUT);
      digitalWrite(relayPin[i], false);
  }
}
void setAllRelayState(bool state){
 for (uint8_t i = 0; i < relayPinCount; i++) {
      digitalWrite(relayPin[i], state);
  }
}
void setRelayState(int index , bool state){
      digitalWrite(relayPin[index], state);
  
}


int delayBetweenPinTest=100;
void TestAllPin(){

for (uint8_t i = 0; i < relayPinCount; i++) {
      setRelayState(i, true);
      delay(delayBetweenPinTest);
  }
for (uint8_t i = 0; i < relayPinCount; i++) {
      setRelayState(i, false);
      delay(delayBetweenPinTest);
  }

}



void fadeTarget(X9C pot) {
  //  force to the end of the wiper.
  for (uint8_t i = 0; i < 99; i++) {
    pot.incr();
    delay(delayTime);
  }

  //  go to the wanted position.  (99 - 24 = 75)
  for (uint8_t i = 0; i < 24; i++) {
    pot.decr();
    delay(delayTime);
  }
}



void potPosition_0_99(X9C pot, float percentRaw) {
  potPosition_0_99(pot, percentRaw * 99);
}


void potPosition_0_99(X9C pot, int position) {
  //  force to the end of the wiper.
  for (uint8_t i = 0; i < 99; i++) {
    pot.incr();
  }
  //  go to the wanted position.  (99 - 24 = 75)
  for (uint8_t i = 0; i < position; i++) {
    pot.decr();
  }
}

const unsigned int MAX_MESSAGE_LENGTH = 20;
char message[MAX_MESSAGE_LENGTH]="1111111111";
unsigned int message_pos = 0;
bool messageToProcess=false;


void setToEmpty(){
  for (int i = 0; i < MAX_MESSAGE_LENGTH; i++) {
    message[i] = '\0';
  }
}

void setup() {
  
  setToEmpty();
  Serial.begin(9600);
  Serial1.begin(9600);


  Serial.println();
  Serial.println("X9C10X_LIB_VERSION");

  potLeftHorizontal.begin(46, 47, 10);  //  pulse, direction, select
  potLeftVertical.begin(48, 49, 10);    //  pulse, direction, select
  potRightTrigger.begin(50, 51, 10);    //  pulse, direction, select
  potLeftTrigger.begin(52, 53, 10);     //  pulse, direction, select


setAllRelayState(true);
setRelayState(5,false);
setRelayState(14,false);
  //  store the current position in NV-RAM.
  //pot.store();
}


void OneAfterEachOther() {

  fadeTarget(potLeftTrigger);
  fadeTarget(potLeftVertical);
  fadeTarget(potLeftHorizontal);
  fadeTarget(potRightTrigger);
}


void TestJoystickAndTriggerOneByeOne(){

delay(400);

    potPosition_0_99(potLeftHorizontal, minJoystickX);
    delay(400);
    potPosition_0_99(potLeftHorizontal, middleJoystickX);
    delay(400);
    potPosition_0_99(potLeftHorizontal, maxJoystickX);

    delay(400);

    potPosition_0_99(potLeftVertical, minJoystickY);
    delay(400);
    potPosition_0_99(potLeftVertical, middleJoystickY);
    delay(400);
    potPosition_0_99(potLeftVertical, maxJoystickY);


    delay(400);


    potPosition_0_99(potLeftTrigger, minTriggerLeft);
    delay(400);
    potPosition_0_99(potLeftTrigger, maxTriggerLeft);


    delay(400);


    potPosition_0_99(potRightTrigger, minTriggerRight);
    delay(400);
    potPosition_0_99(potRightTrigger, maxTriggerRight);


    delay(400);

}

void AllAtOnce() {
  //All at once
  for (uint8_t i = 0; i < 99; i++) {
    potLeftTrigger.incr();
    potRightTrigger.incr();
    potLeftHorizontal.incr();
    potLeftVertical.incr();
    delay(delayTime);
  }

int maxLess=1;
  //  go to the wanted position.  (99 - 24 = 75)
  for (uint8_t i = 0; i < maxLess ; i++) {
    potLeftTrigger.decr();
    potRightTrigger.decr();
    potLeftHorizontal.decr();
    potLeftVertical.decr();
    delay(delayTime);
  }
  delay(100);
}


bool reading=false;

void DealWithMessageReceived( String msg ){
      msg.toUpperCase();
      msg.trim();
      messageToProcess=false;
      Serial.println("P "+ msg );
      Serial.println("C "+ String( message_pos) +"Cl "+ String(msg.length()) +" CT "+ String( MAX_MESSAGE_LENGTH) );
    
      if(msg=="TESTALL"){
        Serial.println("- TESTALL ");
        AllAtOnce();

      }
      if(msg=="TESTSEQUENCE"){
        Serial.println("- TESTSEQUENCE " );
        OneAfterEachOther();

      }
      if(msg.startsWith("TEST")){
        Serial.println("- TEST " );
        TestJoystickAndTriggerOneByeOne();

      }
      if(msg=="TL1") potPosition_0_99(potLeftTrigger,maxTriggerLeft );
      else if(msg=="TL0") potPosition_0_99(potLeftTrigger,0 );
      else if(msg=="TR1") potPosition_0_99(potRightTrigger,maxTriggerLeft );
      else if(msg=="TR0") potPosition_0_99(potRightTrigger,0 );
      
      else if(msg=="jlh+") potPosition_0_99(potLeftHorizontal,maxJoystickX );
      else if(msg=="jlh=") potPosition_0_99(potLeftHorizontal,middleJoystickX );
      else if(msg=="jlh-") potPosition_0_99(potLeftHorizontal,minJoystickX);

      else if(msg=="jlv+") potPosition_0_99(potLeftVertical,maxJoystickY );
      else if(msg=="jlv=") potPosition_0_99(potLeftVertical,middleJoystickY );
      else if(msg=="jlv-") potPosition_0_99(potLeftVertical,minJoystickY);
      
      else if(msg=="jrh+") potPosition_0_99(potLeftHorizontal,maxJoystickX );
      else if(msg=="jrh=") potPosition_0_99(potLeftHorizontal,middleJoystickX );
      else if(msg=="jrh-") potPosition_0_99(potLeftHorizontal,minJoystickX);

      else if(msg=="jrv+") potPosition_0_99(potLeftVertical,maxJoystickY );
      else if(msg=="jrv=") potPosition_0_99(potLeftVertical,middleJoystickY );
      else if(msg=="jrv-") potPosition_0_99(potLeftVertical,minJoystickY);


      setToEmpty();
}


void loop() {

 //Check to see if anything is available in the serial receive buffer
 
 while (Serial.available() > 0)
 {
    if(!reading){
        setToEmpty();
        message_pos=0;
        reading=true;
    }
    char inByte = Serial.read();
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH ) )
    {
      message[message_pos] = inByte;
      message_pos++;
    }
    else
    {
      Serial.println("");
      Serial.println("R "+ String(message));
      DealWithMessageReceived(String(message));
      messageToProcess=true;
      reading=false;
    }
  }
 while (Serial1.available() > 0)
 {
    if(!reading){
        setToEmpty();
        message_pos=0;
        reading=true;
    }
    char inByte = Serial1.read();
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH ) )
    {
      message[message_pos] = inByte;
      message_pos++;
    }
    else
    {
      Serial.println("");
      Serial.println("R "+ String(message));
      Serial1.println("");
      Serial1.println("R "+ String(message));
      DealWithMessageReceived(String(message));
      messageToProcess=true;
      reading=false;
      
    }
  }
 


    TestAllPin();
    
  
}




