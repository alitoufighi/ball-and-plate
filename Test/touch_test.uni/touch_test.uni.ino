/*=================================
This code demostrates 4-Wire Touch screen 
interfacing with Arduino

blog.circuits4you.com
www.circuits4you.com

4- Wire Touchscreen Connections
A0=====X+
A1=====X-
A2=====Y+
A3=====Y-
=================================*/
//Define your Touch screen connections
#define X1 A0
#define X2 A1
#define Y1 A2
#define Y2 A3
//Define your screen resolution as per your Touch screen (Max: 1024)
#define Xresolution 180 //180
#define Yresolution 113 //85

void setup()
{
   Serial.begin(9600);
}

void loop()
{
  int X,Y; //Touch Coordinates are stored in X,Y variable

    // X
    // We are going to measure voltage on Y1 
   pinMode(Y1,INPUT);

   // Make Y2 Tristate 
   pinMode(Y2,INPUT);  
   digitalWrite(Y2,LOW);

   // Form a voltage divider in X1(+5V) and X2(GND) 
   pinMode(X1,OUTPUT);
   digitalWrite(X1,HIGH);
   
   pinMode(X2,OUTPUT);
   digitalWrite(X2,LOW);
   X = (analogRead(Y1))/(1024/Xresolution) - 12; //Reads X axis touch position
//    X = analogRead(Y1);
   pinMode(X1,INPUT);
   pinMode(X2,INPUT);
   digitalWrite(X2,LOW);
   pinMode(Y1,OUTPUT);
   digitalWrite(Y1,HIGH);
   pinMode(Y2,OUTPUT);
   digitalWrite(Y2,LOW);
   Y = (analogRead(X1))/(1024/Yresolution) - 14; //Reads Y axis touch position
//  Y = analogRead(X1);
  //Display X and Y on Serial Monitor
   Serial.print("X = ");  
   Serial.print(X);
   Serial.print(" Y = ");
   Serial.println(Y);
   Serial.println("--------\n");
   delay(100);
}
