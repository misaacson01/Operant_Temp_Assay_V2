//set program ID
uint16_t programIDint = 9340; //ILI9340-based display
uint8_t programNum = 1; //controller #
uint8_t versionID = 1; 

bool rundemo = 0; //for testing displays without scripts

//define pinouts
#define BACKLIGHT 6 //pin which controls the display backlight
#define TRIG_IN 0 //pin which reads input trigger signals
#define TRIG_OUT 1 //pin which sets an output trigger signal
#define TFT_DC 8 //Data/command
#define TFT_CS 10 //chip-select
#define TFT_RST 9 //reset
//#define TFT_MOSI 11 //no need to define SPI pins unless you use non-hardware SPI pins
//#define TFT_MISO 12
//#define TFT_CLK 13

//include necessary libraries
#include "SPI.h"
#include <Adafruit_GFX.h>        // Core graphics library
#include <Adafruit_ILI9341.h>      // Hardware-specific driver library

//create TFT object
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC); 

//create byte/float and byte/int16 unions
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

//initialize pattern parameters
uint8_t commandID, numRepeats, gratingType, wait4trig, tmp;
binaryFloat r1, r2, r3, g1, g2, g3, b1, b2, b3; //R, G, B color values will be 0-1 floats, transferred as bytes
binaryInt xCenter, yCenter, patternWidth, patternHeight, angle, barWidth; //int16s, transferred as bytes
binaryFloat freq, dur, pre; //floats, transferred as bytes

//initialize other common variables
int w, h, x, y, c, on, i, x0, y0, x1, y1, blx, bly, curX, curY, xHeight, xWidth, yHeight, yWidth, displayHeight, displayWidth, bigWidth;
int barWidthLines, bar, numLines, numAddedLines, startLine, nextLine, addedDirection, startC, L, xDir, yDir;
bool xShift[600];
int readDelay = 10;
uint16_t color1, color2, color3, color3_old, colors[3]; //16-bit colors
bool endPattern = 0;
bool backlightState = LOW;
bool xBigger;
long startTime, displayTime, onTime;
unsigned long frameUs, lineUs, startUs, durUs, shiftUs;
double radius;
float slope, blAngle;
float pi = 3.14159;
binaryLong timestamp;
binaryInt programID;


/////////setup (runs once)
void setup() {
  Serial.begin(115200);
  pinMode(BACKLIGHT, OUTPUT);
  pinMode(TRIG_IN, INPUT);
  pinMode(TRIG_OUT, OUTPUT);
  digitalWrite(BACKLIGHT, LOW);
  digitalWrite(TRIG_OUT, LOW);
  digitalWrite(TFT_RST, HIGH);
  tft.begin();
  delay(100);

  programID.integer = programIDint;

  tft.setRotation(3); // for displays mounted sideways, with pins on the left
  displayWidth = tft.width(); // width of display in pixels
  displayHeight = tft.height(); // height of display in pixels
  tft.fillScreen(ILI9341_BLACK);
}

/////////create other small functions
//convert 0-1 RGB values to 16-bit color
uint16_t RGBto16(float R, float G, float B) {
  uint8_t r = round(R*31); //red gets 5 bits
  uint8_t g = round(G*63); //green gets 6 bits
  uint8_t b = round(B*31); //blue gets 5 bits
  uint16_t c = r; //add red to lowest 5 bits 
  c = c<<6; //shift over by 6 bits to make room for green
  c |= g; //add green to lowest 6 bits 
  c = c<<5; //shift over by 5 bits to make room for blue
  c |= b; //add blue to lowest 5 bits 
  return c;
}

//wait for desired amount of available serial data bytes
void wait4Serial(int wait, int num2wait4) {
  while (Serial.available()<num2wait4) {
    delay(wait);
  }
}

//convert degrees to radians
float deg2rad(float deg) {
  return (deg*pi/180); //convert degrees to radians
}

//convert degrees to radians
float rad2deg(float rad) {
  return ((rad*180)/pi); //convert degrees to radians
}


bool check4end() {
  //end pattern if stop command is recieved
  endPattern = 0;
  if (Serial.available()>0) {
    if (Serial.peek()==200) {
      endPattern = 1;
      commandID = Serial.read();
      Serial.write(commandID);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
    } 
  }
  //end pattern if duration is reached
  if ((dur.floatingPoint>0) && ((millis()-startTime)>=(dur.floatingPoint*1000)-1)) {
    endPattern = 1;
  }
  return endPattern;
}


//run display demo without needing PC input (set rundemo=1)
void runDemo() {
  while (1) { //repeat indefinitely
    Serial.println("Beginning fast V lines");
    digitalWrite(BACKLIGHT, HIGH);
    startTime = millis();
    for (x=0;x<displayWidth;x++) {
      tft.drawFastVLine(x, 0, displayHeight, 15);
    }
    for (x=0;x<displayWidth;x++) {
      tft.drawFastVLine(x, 0, displayHeight, 0);
    }
    digitalWrite(BACKLIGHT, LOW);
    displayTime = millis()-startTime;
    Serial.print("elapsed time: ");
    Serial.print((displayTime));
    Serial.println("ms");
    delay(2000);
  }
}


//////// main program loop
void loop() {
  if (rundemo==1) {runDemo();}
  
  //wait for incoming instructions over serial connection
  endPattern = 0;
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
      
    case 101: //turn on backlight
      backlightState = HIGH;
      timestamp.longInteger = millis();
      digitalWrite(BACKLIGHT, backlightState);

      //send back command for verification
      Serial.write(commandID);
      Serial.write(timestamp.binary,4);
      
      break;

    case 102: //turn off backlight
      backlightState = LOW;
      timestamp.longInteger = millis();
      digitalWrite(BACKLIGHT, backlightState);

      //send back command for verification
      Serial.write(commandID);
      Serial.write(timestamp.binary,4);
      break;
    
    case 103: //command to display a flickering rectangle
      wait4Serial(readDelay, 45); //wait for 45 available bytes

      //read pattern parameters
      for (i=0;i<4;i++) {r1.binary[i] = Serial.read();} //bytes 1-4
      for (i=0;i<4;i++) {g1.binary[i] = Serial.read();} //bytes 5-8
      for (i=0;i<4;i++) {b1.binary[i] = Serial.read();} //bytes 9-12
      for (i=0;i<4;i++) {r3.binary[i] = Serial.read();} //bytes 13-16
      for (i=0;i<4;i++) {g3.binary[i] = Serial.read();} //bytes 17-20
      for (i=0;i<4;i++) {b3.binary[i] = Serial.read();} //bytes 21-24
      for (i=0;i<2;i++) {xCenter.binary[i] = Serial.read();} //bytes 25-26
      for (i=0;i<2;i++) {yCenter.binary[i] = Serial.read();} //bytes 27-28
      for (i=0;i<2;i++) {patternWidth.binary[i] = Serial.read();} //bytes 29-30
      for (i=0;i<2;i++) {patternHeight.binary[i] = Serial.read();} //bytes 31-32
      for (i=0;i<4;i++) {freq.binary[i] = Serial.read();} //bytes 33-36
      for (i=0;i<4;i++) {dur.binary[i] = Serial.read();} //bytes 37-40
      for (i=0;i<4;i++) {pre.binary[i] = Serial.read();} //bytes 41-44
      wait4trig = Serial.read(); //byte 45

      //calculate backlight on time (if flickering)
      if (freq.floatingPoint>0) {
        onTime = round(500/freq.floatingPoint);
      }

      //set background color
      color3 = RGBto16(r3.floatingPoint,g3.floatingPoint,b3.floatingPoint);
      if (color3!=color3_old) {
        tft.fillScreen(color3);
        color3_old = color3;
      }
      
      //send pattern parameters back for verification
      Serial.write(commandID);
      Serial.write(r1.binary,4);
      Serial.write(g1.binary,4);
      Serial.write(b1.binary,4);
      Serial.write(r3.binary,4);
      Serial.write(g3.binary,4);
      Serial.write(b3.binary,4);
      Serial.write(xCenter.binary,2);
      Serial.write(yCenter.binary,2);
      Serial.write(patternWidth.binary,2);
      Serial.write(patternHeight.binary,2);
      Serial.write(freq.binary,4);
      Serial.write(dur.binary,4);
      Serial.write(pre.binary,4);
      Serial.write(wait4trig);
      startTime = millis();
      timestamp.longInteger = startTime;
      Serial.write(timestamp.binary,4); //46-49

      //wait for input trigger (if applicable)
      if (wait4trig==1) {
        while (wait4trig==1) {
          if (Serial.available()>0) { //if any incoming commands are detected, abort current pattern
            endPattern = 1;
            wait4trig = 0;
          }
          if (digitalRead(TRIG_IN)==HIGH) { //if input trigger signal goes high
            wait4trig = 0;
            Serial.write(150); //ID for trigger
            startTime = millis();
            timestamp.longInteger = startTime;
            Serial.write(timestamp.binary,4); 
          }
          delay(1); // check inputs every 1 ms
        }
      }

      //wait if predelay is specified
      if ((millis()-startTime)<(1000*pre.floatingPoint)) {
        delay((startTime+(pre.floatingPoint*1000))-(millis()-startTime));
      }
      
      //turn on backlight, start counting duration
      digitalWrite(BACKLIGHT, HIGH);
      digitalWrite(TRIG_OUT, HIGH);
      startTime = millis();

      //draw rectangle
      endPattern = 0;
      color1 = RGBto16(r1.floatingPoint,g1.floatingPoint,b1.floatingPoint);
      x0 = xCenter.integer-0.5-round(patternWidth.integer/2);
      //x1 = xCenter.integer-0.5+round(patternWidth.integer/2); //don't need x1 for H lines, only width
      y0 = yCenter.integer-0.5-round(patternHeight.integer/2);
      y1 = yCenter.integer-0.5+round(patternHeight.integer/2);
      for (y=y0;y<=y1;y++) {
        tft.drawFastHLine(x0, y, patternWidth.integer, color1);
      }

      //start delay/flicker loop
      while (endPattern==0) {
        displayTime = millis()-startTime;
        
        //turn backlight on/off according to flicker frequency
        if (freq.floatingPoint>0) {
          on = floor(displayTime/onTime);
          if ((on%2)==0) {
            digitalWrite(BACKLIGHT, HIGH);
          } else {
            digitalWrite(BACKLIGHT, LOW);
          }
        }

        //check if pattern end condition is reached
        if (endPattern==0) {endPattern = check4end();}

        delay(1);
      }

      //set backlight back to original state, set display to background color
      digitalWrite(BACKLIGHT, backlightState);
      digitalWrite(TRIG_OUT, LOW);
      tft.fillScreen(color3);
      break;


    case 104: //display fast gratings (only leading/trailing edges are drawn every frame)
      wait4Serial(readDelay, 62);

      //read pattern parameters
      for (i=0;i<4;i++) {r1.binary[i] = Serial.read();} //bytes 1-4
      for (i=0;i<4;i++) {g1.binary[i] = Serial.read();} //bytes 5-8
      for (i=0;i<4;i++) {b1.binary[i] = Serial.read();} //bytes 9-12
      for (i=0;i<4;i++) {r2.binary[i] = Serial.read();} //bytes 13-16
      for (i=0;i<4;i++) {g2.binary[i] = Serial.read();} //bytes 17-20
      for (i=0;i<4;i++) {b2.binary[i] = Serial.read();} //bytes 21-24
      for (i=0;i<4;i++) {r3.binary[i] = Serial.read();} //bytes 25-28
      for (i=0;i<4;i++) {g3.binary[i] = Serial.read();} //bytes 29-32
      for (i=0;i<4;i++) {b3.binary[i] = Serial.read();} //bytes 33-36
      for (i=0;i<2;i++) {xCenter.binary[i] = Serial.read();} //bytes 37-38
      for (i=0;i<2;i++) {yCenter.binary[i] = Serial.read();} //bytes 39-40
      for (i=0;i<2;i++) {patternWidth.binary[i] = Serial.read();} //bytes 41-42
      for (i=0;i<2;i++) {patternHeight.binary[i] = Serial.read();} //bytes 43-44
      for (i=0;i<2;i++) {barWidth.binary[i] = Serial.read();} //bytes 45-46
      for (i=0;i<2;i++) {angle.binary[i] = Serial.read();} //bytes 47-48
      for (i=0;i<4;i++) {freq.binary[i] = Serial.read();} //bytes 49-52
      for (i=0;i<4;i++) {dur.binary[i] = Serial.read();} //bytes 53-56
      for (i=0;i<4;i++) {pre.binary[i] = Serial.read();} //bytes 57-60
      gratingType = Serial.read(); //byte 61
      wait4trig = Serial.read(); //byte 62

      //get location of every pixel (x0[],y0[]) which creates an 
      //orthogonally-connected line primitive closest to the "bottom line"
      //method: iterate by walking 1 pixel at a time, orthogonally only, first by 
      //using the direction with the greater overall change (dx or dy) then by 
      //minimizing the error to the true line. 

      //need to find corner coordinate to begin line. find bottom-left corner first
      radius = sqrt(sq(float(patternWidth.integer)/2) + sq(float(patternHeight.integer)/2));
      blAngle = 180+rad2deg(float(atan2(float(patternHeight.integer),float(patternWidth.integer))));
      
      blx = round(xCenter.integer+radius*cos(deg2rad(blAngle-float(angle.integer)))); //bottom-left corner x-coord
      xHeight = round(float(patternHeight.integer-1)*sin(deg2rad(angle.integer)));
      xWidth = round(float(patternWidth.integer-1)*cos(deg2rad(angle.integer)));

      //
      bly = 240-round(yCenter.integer+radius*sin(deg2rad(blAngle-float(angle.integer)))); //bottom-left corner y-coord
      yHeight = round(float(patternHeight.integer-1)*cos(deg2rad(angle.integer)));
      yWidth = round(float(patternWidth.integer-1)*sin(deg2rad(angle.integer)));
      
      Serial.write(201);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      Serial.print("blx, bly: ");
      Serial.print(blx);
      Serial.print(" ");
      Serial.print(bly);
      Serial.print("\n");

      Serial.write(201);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      Serial.print("xHeight, xWidth: ");
      Serial.print(xHeight);
      Serial.print(" ");
      Serial.print(xWidth);
      Serial.print("\n");

      Serial.write(201);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      Serial.print("yHeight, yWidth: ");
      Serial.print(yHeight);
      Serial.print(" ");
      Serial.print(yWidth);
      Serial.print("\n");
      
      if (xWidth>=yWidth) {
        xBigger = 1;
        bigWidth = xWidth;
        slope = float(yWidth)/float(xWidth); //1 or -1 if equal, 0 if none
      } else { 
        xBigger = 0;
        bigWidth = yWidth;
        slope = float(xWidth)/float(yWidth); //1 or -1 if equal, 0 if none
      }
      if ((angle.integer<=90) || (angle.integer>=270)) {
        xDir = 1;
      } else {
        xDir = -1;
      }
      if (angle.integer<=180) {
        yDir = 1;
      } else {
        xDir = -1;
      }
      addedDirection = round((slope/abs(slope))+0.0001);
      numLines = 0;
      numAddedLines = 0;
      for (i=0;i<abs(bigWidth);i++) { 
        xShift[numLines] = xBigger; //shift in the bigger direction (1=x, 0=y)
        numLines++;

        //check for additional pixel to add in the y direction
        if ((abs(round(slope*i))-numAddedLines)==1) {
          xShift[numLines] = 1-xBigger; //shift in the smaller direction (same as before, 1=x, 0=y)
          numLines++;
          numAddedLines++;
        }
      }

      Serial.write(201);
      timestamp.longInteger = millis();
      Serial.write(timestamp.binary,4);
      Serial.print("x, y dir: ");
      Serial.print(xDir);
      Serial.print(" ");
      Serial.print(yDir);
      Serial.print("\n");
      
      frameUs = 1000000*(0.5/freq.floatingPoint); //duration to update a frame for desired frequency (in microseconds)
      lineUs = frameUs/float(numLines); //duration to update a line for desired frequency
      color1 = RGBto16(r1.floatingPoint,g1.floatingPoint,b1.floatingPoint);
      color2 = RGBto16(r2.floatingPoint,g2.floatingPoint,b2.floatingPoint);
      color3 = RGBto16(r3.floatingPoint,g3.floatingPoint,b3.floatingPoint);
      colors[0] = color1;
      colors[1] = color2;
      colors[2] = color3;
      barWidthLines = round((float(barWidth.integer)/float(patternWidth.integer))*float(numLines));
      curX = blx;
      curY = bly;
      
      //set background color
      color3 = RGBto16(r3.floatingPoint,g3.floatingPoint,b3.floatingPoint);
      if (color3!=color3_old) {
        tft.fillScreen(color3);
        color3_old = color3;
      }

      //send pattern parameters back for verification
      Serial.write(commandID);
      Serial.write(r1.binary,4);
      Serial.write(g1.binary,4);
      Serial.write(b1.binary,4);
      Serial.write(r2.binary,4);
      Serial.write(g2.binary,4);
      Serial.write(b2.binary,4);
      Serial.write(r3.binary,4);
      Serial.write(g3.binary,4);
      Serial.write(b3.binary,4);
      Serial.write(xCenter.binary,2);
      Serial.write(yCenter.binary,2);
      Serial.write(patternWidth.binary,2);
      Serial.write(patternHeight.binary,2);
      Serial.write(barWidth.binary,2);
      Serial.write(angle.binary,2);
      Serial.write(freq.binary,4);
      Serial.write(dur.binary,4);
      Serial.write(pre.binary,4);
      Serial.write(gratingType);
      Serial.write(wait4trig);
      startTime = millis(); 
      timestamp.longInteger = startTime;
      Serial.write(timestamp.binary,4); //63-66
      
      //wait for input trigger (if applicable)
      if (wait4trig==1) {
        while (wait4trig==1) {
          if (Serial.available()>0) { //if any incoming commands are detected, abort current pattern
            endPattern = 1;
            wait4trig = 0;
          }
          if (digitalRead(TRIG_IN)==HIGH) { //if input trigger signal goes high
            wait4trig = 0;
            Serial.write(150); //ID for trigger
            startTime = millis();
            timestamp.longInteger = startTime;
            Serial.write(timestamp.binary,4); 
          }
          delay(1); // check inputs every 1 ms
        }
      }
    
      //wait if predelay is specified
      if ((millis()-startTime)<(1000*pre.floatingPoint)) {
        delay((startTime+(pre.floatingPoint*1000))-(millis()-startTime));
      }

      //turn on backlight, start counting duration
      digitalWrite(BACKLIGHT, HIGH);
      digitalWrite(TRIG_OUT, HIGH);
      startTime = millis();

      switch (gratingType) {
        case 0: //flicker
          startUs = micros();
          c=1;
          while (endPattern==0) {
            curX = blx;
            curY = bly;
            for (i=0;i<numLines;i++) {
              tft.drawLine(curX, curY, curX+xHeight, curY-yHeight, colors[c]); 
              curX += xDir*xShift[i];
              curY += yDir*(1-xShift[i]);
            }
            //check if pattern end condition is reached
            if (endPattern==0) {endPattern = check4end();}
                
            //wait until frame-period is finished
            durUs = micros()-startUs;
            if ((endPattern==0) && (durUs<frameUs)) {
              if ((frameUs-durUs)>16380) {
                delay((frameUs-durUs)/1000);
              } else {
                delayMicroseconds(frameUs-durUs);
              }
            }
            
            c=1-c; //switch color
          }

          //draw background color
          curX = blx;
          curY = bly;
          for (i=0;i<numLines;i++) {
            tft.drawLine(curX, curY, curX+xHeight, curY-yHeight, color3); 
            curX += xDir*xShift[i];
            curY += yDir*(1-xShift[i]);
          }
          break;
        
        case 1: //square-wave gratings
          
          //draw starting grating pattern
          c = 0;
          nextLine = 1;
          for (i=0;i<numLines;i++) {
            tft.drawLine(curX, curY, curX+xHeight, curY-yHeight, colors[c]); 
            curX += xDir*xShift[i];
            curY += yDir*(1-xShift[i]);
            nextLine++;
            if (nextLine>barWidthLines) { //if bar is finished, switch to other color and start drawing new bar over
              nextLine = 1;
              c = 1-c;
            }
          }
          
          //update edges during pattern
          while (endPattern==0) {
            for (startC=0;startC<=1;startC++) {
              for (startLine=0;startLine<barWidthLines;startLine++) {
                c = 1-startC;
                startUs = micros();
                for (L=startLine;L<numLines;L=L+barWidthLines) {
                  curX = blx;
                  curY = bly;
                  if (L>0) {
                    for (i=0;i<L;i++) {
                      curX += xDir*xShift[i];
                      curY += yDir*(1-xShift[i]);
                    }
                  }
                  tft.drawLine(curX, curY, curX+xHeight, curY-yHeight, colors[c]); 
                  c = 1-c;
                }

                //check if pattern end condition is reached
                if (endPattern==0) {endPattern = check4end();}
                
                //wait until line-period is finished
                durUs = micros()-startUs;
                if ((endPattern==0) && (durUs<lineUs)) {
                  if ((lineUs-durUs)>16380) {
                    delay((lineUs-durUs)/1000);
                  } else {
                    delayMicroseconds(lineUs-durUs);
                  }
                }
              }
            }
          }

          //draw background again
          curX = blx;
          curY = bly;
          for (i=0;i<numLines;i++) {
            tft.drawLine(curX, curY, curX+xHeight, curY-yHeight, colors[2]); 
            curX += xDir*xShift[i];
            curY += yDir*(1-xShift[i]);
          }
          break;
      }

      //set backlight back to original state, set trig output to low
      digitalWrite(BACKLIGHT, backlightState);
      digitalWrite(TRIG_OUT, LOW);
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
