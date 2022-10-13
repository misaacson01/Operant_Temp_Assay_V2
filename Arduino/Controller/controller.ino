//set program ID
uint16_t programIDint = 1000; //controller 1000 (thermoV2 controller)
uint8_t programNum = 1; //controller #1
uint8_t versionID = 1; //V1

bool rundemo = 0; //for testing displays without scripts

//include necessary libraries
#include "SPI.h"
#include <Adafruit_MAX31856.h>

//pinouts
uint8_t relayPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
uint8_t sensorPins[3] {A0, A1, A2};
#define CS 10 //chip-select
//#define TFT_MOSI 11 //no need to define SPI pins unless you use non-hardware SPI pins
//#define TFT_MISO 12
//#define TFT_CLK 13
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(CS);

typedef union {
  float floatingPoint;
  byte binary[4];
} binaryFloat;

typedef union {
  int integer;
  byte binary[2];
} binaryInt;

typedef union {
  long longInteger;
  byte binary[4];
} binaryLong;

//initialize variables
uint8_t pin, commandID, fault, tmp;
int sensorVal;
float curtemp;
binaryLong curT, timestamp;
binaryFloat dur, temp;
binaryInt programID, absoluteSensorVal;
long startTime;
int i, j;
int readDelay = 10;
bool endCommand = 0;


//setup (runs once)
void setup() {
  programID.integer = programIDint;
  Serial.begin(115200);

  //set output pins
  for (i=0;i<8;i++) {
      pinMode(relayPins[i], OUTPUT);
      delay(1);
      digitalWrite(relayPins[i], HIGH); //HIGH = off
  }

  //initialize thermocouple
  if (!maxthermo.begin()) {
    Serial.println("Could not initialize thermocouple.");
    while (1) delay(10);
  }
  maxthermo.setThermocoupleType(MAX31856_TCTYPE_K);
}


//wait for desired amount of available serial data bytes
void wait4Serial(int wait, int num2wait4) {
  while (Serial.available()<num2wait4) {
    delay(wait);
  }
}

bool check4end() {
  //end current command if stop command is recieved
  endCommand = 0;
  if (Serial.available()>0) {
    if (Serial.peek()==200) {
      endCommand = 1;
      commandID = Serial.read();
      Serial.write(commandID);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
    } 
  }
  //end pattern if duration is reached
  if ((dur.floatingPoint>0) && ((millis()-startTime)>=(dur.floatingPoint*1000)-1)) {
    endCommand = 1;
  }
  return endCommand;
}
void runDemo() {
  while (1) { //repeat indefinitely

    Serial.println("Testing relays");
    Serial.print("Pins: ");
    for (i=0;i<8;i++) {
      digitalWrite(relayPins[i], LOW);
      Serial.print(relayPins[i]);
      Serial.print(" ");
      delay(500);
      digitalWrite(relayPins[i], HIGH);
    }
    Serial.println("");
    
    Serial.println("Testing sensors");
    Serial.print("Pins: ");
    for (i=0;i<3;i++) {
      sensorVal = analogRead(sensorPins[i]);
      Serial.print(sensorPins[i]);
      Serial.print(": ");
      Serial.print(sensorVal);
      Serial.print("    ");
    }
    Serial.println("");
    delay(1000);
  }
}


void loop() { //main program loop
  if (rundemo==1) {runDemo();} //self-explanatory, right?
  
  //wait for incoming instructions over serial connection
  wait4Serial(readDelay, 1); //wait for (at least) 1 available byte
  
  //read incoming command and data over serial
  commandID = Serial.read(); //read incoming message ID byte (specifies what data is coming next)
  switch (commandID) {
    case 0: //command to send version ID back over serial
      Serial.write(commandID);
      Serial.write(programID.binary,2); //send program ID as 2 bytes
      Serial.write(programNum);
      Serial.write(versionID);
      break;
      
    case 1: //send timestamp
      Serial.write(commandID);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      break;
      
    case 120: //command to toggle a relay for a set duration
      wait4Serial(readDelay, 5); //wait for 5 available bytes

      //read parameters
      dur.binary[0] = Serial.read(); //byte 1
      dur.binary[1] = Serial.read(); //byte 2
      dur.binary[2] = Serial.read(); //byte 3
      dur.binary[3] = Serial.read(); //byte 4
      pin = Serial.read(); //byte 5
      
      //send pattern parameters back for verification
      Serial.write(commandID);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      Serial.write(dur.binary,4);
      Serial.write(pin);

      digitalWrite(relayPins[pin], LOW);
      delay(1000*dur.floatingPoint);
      digitalWrite(relayPins[pin], HIGH);
      break;

    case 121: //comand to read the temperature for a set duration
      wait4Serial(readDelay, 4); //wait for 4 available bytes

      //read parameters
      dur.binary[0] = Serial.read(); //byte 1
      dur.binary[1] = Serial.read(); //byte 2
      dur.binary[2] = Serial.read(); //byte 3
      dur.binary[3] = Serial.read(); //byte 4

      //send parameters back for verification
      Serial.write(commandID);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      Serial.write(dur.binary,4);

      startTime = millis();
      curT.longInteger = 0;
      endCommand = 0;
      while (endCommand==0) {
        curT.longInteger=millis();
        curtemp = maxthermo.readThermocoupleTemperature();
        temp.floatingPoint = curtemp;
        Serial.write(151); //ID for temperature reading
        Serial.write(curT.binary,4);
        Serial.write(temp.binary,4);
        endCommand = check4end();
        delay(100);
      }
      fault = maxthermo.readFault();
      if (fault) {
        curT.longInteger=1000*(millis()-startTime);
        Serial.write(255);
        Serial.write(curT.binary,4);
        if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
        if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
        if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
        if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
        if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
        if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
        if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
        if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
      }
      break;

    case 122: //command to read the sensors for a set duration, until the first detection, or until stop command
      wait4Serial(readDelay, 4); //wait for 4 available bytes

      //read parameters
      dur.binary[0] = Serial.read(); //byte 1
      dur.binary[1] = Serial.read(); //byte 2
      dur.binary[2] = Serial.read(); //byte 3
      dur.binary[3] = Serial.read(); //byte 4

      //send pattern parameters back for verification
      Serial.write(commandID);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      Serial.write(dur.binary,4);

      startTime = millis();
      curT.longInteger = 0;
      endCommand=0;
      while (endCommand==0) {
        curT.longInteger=millis();
        for (i=0;i<3;i++) {
          sensorVal = analogRead(sensorPins[i]);
          if (sensorVal<512) {
            Serial.write(152);
            Serial.write(curT.binary,4);
            Serial.write(i);
            endCommand=1;
            break;
          }
        }
        delay(2);
        if (endCommand==0) {
          endCommand = check4end();
        }
      }
      break;

    case 123: //get absolute sensor readings
      Serial.write(commandID);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      for (j=0;j<5;j++) {
        for (i=0;i<3;i++) {
          absoluteSensorVal.integer = analogRead(sensorPins[i]);
          Serial.write(153);
          Serial.write(absoluteSensorVal.binary,2);
          Serial.write(byte(i));
        }
        delay(1000);
      }
      break;
      
    case 200: //stop command recieved
      Serial.write(commandID);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      break;

      
    default: //send back unknown command
      Serial.write(commandID);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      Serial.print("unknown bytes: ");
      while (Serial.available()>0) {
        tmp = Serial.read();
        Serial.print(tmp);
        Serial.print(" ");
      }
      Serial.print("\n");
      break;
  }
}
//loop end
