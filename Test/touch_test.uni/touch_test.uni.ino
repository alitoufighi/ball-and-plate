///*=================================
//This code demostrates 4-Wire Touch screen 
//interfacing with Arduino
//
//blog.circuits4you.com
//www.circuits4you.com
//
//4- Wire Touchscreen Connections
//A0=====X+
//A1=====X-
//A2=====Y+
//A3=====Y-
//=================================*/
////Define your Touch screen connections
//#define X1 A0
//#define X2 A1
//#define Y1 A2
//#define Y2 A3
////Define your screen resolution as per your Touch screen (Max: 1024)
//#define Xresolution 200 //max: 174 [0:174] mid:87
//#define Yresolution 113 //max: 84  [0:84]  mid:42
//// ratio is 16:9
//#include "TouchScreen.h"
//
//void setup()
//{
//   Serial.begin(9600);
//}
//
//void loop()
//{
//  int X,Y; //Touch Coordinates are stored in X,Y variable
//
//    // X
//    // We are going to measure voltage on Y1 
//   pinMode(Y1,INPUT);
//
//   // Make Y2 Tristate 
//   pinMode(Y2,INPUT);  
//   digitalWrite(Y2,LOW);
//
//   // Form a voltage divider in X1(+5V) and X2(GND) 
//   pinMode(X1,OUTPUT);
//   digitalWrite(X1,HIGH);
//   
//   pinMode(X2,OUTPUT);
//   digitalWrite(X2,LOW);
//   X = (analogRead(Y1))/(1024/Xresolution) - 12; //Reads X axis touch position
////    X = analogRead(Y1);
//   pinMode(X1,INPUT);
//   pinMode(X2,INPUT);
//   digitalWrite(X2,LOW);
//   pinMode(Y1,OUTPUT);
//   digitalWrite(Y1,HIGH);
//   pinMode(Y2,OUTPUT);
//   digitalWrite(Y2,LOW);
//   Y = (analogRead(X1))/(1024/Yresolution) - 16; //Reads Y axis touch position
////  Y = analogRead(X1);
//  //Display X and Y on Serial Monitor
//   Serial.print("X = ");  
//   Serial.print(X);
//   Serial.print(" Y = ");
//   Serial.println(Y);
//   Serial.println("--------\n");
//   delay(100);
//}

// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// This demo code returns raw readings, public domain

#include <stdint.h>
#include "TouchScreen.h"

#define YP A0  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM A1   // can be a digital pin
#define XP A3   // can be a digital pin

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 900);

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  // a point object holds x y and z coordinates
  double t1 = micros();
  TSPoint p = ts.getPoint();
  Serial.println(micros() - t1);
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  
//  if (p.z > ts.pressureThreshhold) {
//     int X = (p.x - 68) * 16 / 25; //max=571
//     int Y = (p.y - 142) * 9 / 25; //max=274
////     Serial.print("X = "); Serial.print(X);
////     Serial.print("\tY = "); Serial.print(Y);
////     Serial.print("\tPressure = "); Serial.println(p.z);
//  }

  delay(100);
}
