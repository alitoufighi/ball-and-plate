#include <PID_v1.h>
#include <stdint.h>
#include <TouchScreen.h>
#include <Wire.h>
#include <Servo.h>

// Definitions TOUCH PINS
#define YP A0
#define XM A2
#define YM A1
#define XP A3

#define SERVO1_PIN 5
#define SERVO2_PIN 6

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 900);

unsigned int noTouchCount = 0; //viariable for noTouch

// PID values
double Setpoint1, Input1, Output1;     //for X
double Setpoint2, Input2, Output2;  //for Y

// servos variables
Servo servo1; //X axis
Servo servo2; //Y axis

float convert1 = 0.3;  // converts raw x values to mm. found through manual calibration
float convert2 = 0.3;   // converts raw y values to mm. found through manual calibration

int flatX = 79;
int flatY = 67;

const int MAX_X = 571 * convert1;
const int MAX_Y = 2110 * convert2;

const int BORDER_X = 30;
const int BORDER_Y = 17;


const float MID_X = 286 * convert1;
const float MID_Y = 105 * convert2;
const float OFFSET_Y = 25;
const float OFFSET_X = 60;

/////TIME SAMPLE
int Ts = 100; 
unsigned long Stable=0; 
//PID const
float Kp1 = 0.5;                                                     
float Ki1 = 0.0;                                                      
float Kd1 = 0.25;

float Kp2 = 0.2;                                                       
float Ki2 = 0.0;                                                      
float Kd2 = 0.1;

//INIT PID
PID myPID1(&Input1, &Output1, &Setpoint1, Kp1, Ki1, Kd1, DIRECT);
PID myPID2(&Input2, &Output2, &Setpoint2, Kp2, Ki2, Kd2, DIRECT);

#define SWITCH_SP 2

double k = 0;
float delta = 0.01;

void setup()
{
    pinMode(SWITCH_SP, INPUT);
    
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    
    //INIT PINS
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(9600);
    
    //INIT Setpoint1
    Setpoint1 = MID_X;
    Setpoint2 = MID_Y;
    
    //// Make plate flat
    Output1 = flatX;
    Output2 = flatY;
    servo1.write(Output1);
    servo2.write(Output2);
    
    //Zapnutie PID
    myPID1.SetMode(AUTOMATIC);
    myPID1.SetOutputLimits(19, 159);
    myPID2.SetMode(AUTOMATIC);
    myPID2.SetOutputLimits(17, 117);
    // TIME SAMPLE
    myPID2.SetSampleTime(Ts); 
    myPID1.SetSampleTime(Ts);  
    /////
    delay(100);
    ///
}

void switch_sp(){
  static char x = '\0';
  if(Serial.available() > 0) {
      x = Serial.read();
      if (x == 'q') {
        Kp1 += delta;
      } else if (x == 'w') {
        Kd1 += delta;
  //          } else if (x == 'e') {
  //            Kd1 += delta;
      } else if (x == 'a') {
        Kp1 -= delta;
      } else if (x == 's') {
        Kd1 -= delta;
  //          } else if (x == 'd') {
  //            Kd1 -= delta;
      } else if (x == 'p') {
        Kp2 += delta;
      } else if (x == '[') {
        Kd2 += delta;
  //          } else if (x == ']') {
  //            Kd2 += delta;
      } else if (x == 'l') {
        Kp2 -= delta;
      } else if (x == ';') {
        Kd2 -= delta;
  //          } else if (x == '\'') {
  //            Kd2 -= delta;
  //          } else if (x == '1') {
  //            delta += 0.01;
  //          } else if (x == '2') {
  //            delta -= 0.01;
      } else if (x == '3') {
        Setpoint1 = MID_X - OFFSET_X;
        Setpoint2 = MID_Y - OFFSET_Y;
      } else if (x == '2') {
        Setpoint1 = MID_X - OFFSET_X;
        Setpoint2 = MID_Y;
      } else if (x == '6') {
        Setpoint1 = MID_X;
        Setpoint2 = MID_Y - OFFSET_Y;
      } else if (x == '4') {
        Setpoint1 = MID_X;
        Setpoint2 = MID_Y + OFFSET_Y;
      } else if (x == '8') {
        Setpoint1 = MID_X + OFFSET_X;
        Setpoint2 = MID_Y;
      } else if (x == '1') {
        Setpoint1 = MID_X - OFFSET_X;
        Setpoint2 = MID_Y + OFFSET_Y;
      } else if (x == '5') {
        Setpoint1 = MID_X;
        Setpoint2 = MID_Y;
      } else if (x == '9') {
        Setpoint1 = MID_X + OFFSET_X;
        Setpoint2 = MID_Y - OFFSET_Y;
      } else if (x == '7') {
        Setpoint1 = MID_X + OFFSET_X;
        Setpoint2 = MID_Y + OFFSET_Y;
      }
      
      myPID1.SetTunings(Kp1, Ki1, Kd1);
      myPID2.SetTunings(Kp2, Ki2, Kd2); 
      
      Serial.print("Delta="); Serial.print(delta);
      Serial.print(", Kp1="); Serial.print(Kp1);
  //          Serial.print(", Ki1="); Serial.print(Ki1);
      Serial.print(", Kd1="); Serial.print(Kd1);
      Serial.print(", Kp2="); Serial.print(Kp2);
  //          Serial.print(", Ki2="); Serial.print(Ki2);
      Serial.print(", Kd2="); Serial.println(Kd2);
   }

  if (x == '0') {
    Setpoint1 = MID_X + 25 * cos(k);
    Setpoint2 = MID_Y + 25 * sin(k);
    k=k-0.0075; 
  } else if (x == '*') {
    Setpoint1 = MID_X + (80*cos(k))/(1+sin(k)*sin(k));
    Setpoint2 = MID_Y + (70*sin(k)*cos(k))/(1+sin(k)*sin(k));
    k=k+0.005; 
  }
//  const int OFFSET = 10;
//  static int mode = 0;
//  static int changed = 0;
//  
//  if(digitalRead(SWITCH_SP) == HIGH){
//    Serial.println("DIGITAL HIGH");
//    if(changed == 0){
//      changed = 1;
//      mode = (mode + 1) % 4;
//      Serial.print("-------------CHANGED MODE! MODE IS ");
//      Serial.println(mode);
//    }
//  } else {
//    changed = 0;
//  }
//  
//  switch(mode){
//    case 0:
//      Setpoint1 = 286 * convert1 - OFFSET;
//      Setpoint2 = 137 * convert2 - OFFSET;
//      break;
//    case 1:
//      Setpoint1 = 286 * convert1 - OFFSET;
//      Setpoint2 = 137 * convert2 + OFFSET;
//      break;
//    case 2:
//      Setpoint1 = 286 * convert1 + OFFSET;
//      Setpoint2 = 137 * convert2 + OFFSET;
//      break;
//    case 3:
//      Setpoint1 = 286 * convert1 + OFFSET;
//      Setpoint2 = 137 * convert2 - OFFSET;
//      break;
//  }
}

void loop()
{
    
    
    while(Stable<50) //REGULATION LOOP
    {
       
        switch_sp();
        
        TSPoint p = ts.getPoint();   //measure pressure on plate
//        switch_sp();
        
        if (p.z > ts.pressureThreshhold) //ball is on plate
        {
            servo1.attach(SERVO1_PIN); //connect servos
            servo2.attach(SERVO2_PIN); 
            // setDesiredPosition();
            
            noTouchCount = 0;  
            TSPoint p = ts.getPoint(); // measure actual position 
            int X = (p.x - 68) * 16 / 25; //max=571, mid=286
            int Y = 274 - ((p.y - 142) * 9.0 / 25.0); //max=274, mid=137
//            Serial.print(X); Serial.print(" "); Serial.println(Y);
            Input1 = X * convert1;
            Input2 = Y * convert2;

            if (Input1 > Setpoint1-2 && Input1 < Setpoint1+2 && Input2 > Setpoint2-2 && Input2 < Setpoint2+2){
              Stable++;
              digitalWrite(LED_BUILTIN, HIGH); 
            }
            else {
              Stable = 0;
              digitalWrite(LED_BUILTIN, LOW); 
            }
            
            // change PID at center
//            if(Input1 > Setpoint1-20 && Input1 < Setpoint1+20 && Input2 > Setpoint2-10 && Input2 < Setpoint2+10) //if ball is close to Setpoint1
//            {
//                myPID1.SetTunings(cKp1, cKi1, cKd1);
//                myPID2.SetTunings(cKp2, cKi2, cKd2);
//            }
//            // change PID at borders
////            else if(Input1 < BORDER_X || Input1 > MAX_X - BORDER_X || Input2 > BORDER_Y || Input2 < MAX_Y - BORDER_Y) //if ball is close to Setpoint1
////            {
////                myPID1.SetTunings(bKp1, bKi1, bKd1);
////                myPID2.SetTunings(bKp2, bKi2, bKd2);
////            }
//            else
//            {
//                myPID1.SetTunings(Kp1, Ki1, Kd1);
//                myPID2.SetTunings(Kp2, Ki2, Kd2); 
//            }
            
            myPID1.Compute();  //action control X compute
            myPID2.Compute();  // action control  Y compute   
//            Serial.print(Setpoint1);
//            Serial.print(" <-Setpoint1, Setpoint2-> ");
//            Serial.println(Setpoint2);
//            Serial.print("Input1: ");
//            Serial.print(Input1);
//            Serial.print(",Input2: ");
//            Serial.println(Input2); 
//            Serial.print("Output1: ");
//            Serial.print(Output1);
//            Serial.print(",Output2: ");
//            Serial.println(Output2);
//            Serial.println("-------");
        }
        else //if there is no ball on plate
        {
            noTouchCount++; //increment no touch count

            if(noTouchCount >= 300) 
            {
                Output1=flatX; //make plate flat
                Output2=flatY;
            }
            if(noTouchCount >= 800) //if there is no ball on plate longer
            {
                servo1.detach(); //detach servos
                servo2.detach();     
            }
        }
        
        servo1.write(Output1);//control
        servo2.write(Output2);//control 
            
    }////END OF REGULATION LOOP///

    servo1.detach();//detach servos
    servo2.detach();

    ///KONTROLA STABILITY////
    while(Stable == 50)//if is stable
    { //still measure actual postiion
        // setDesiredPosition(); 
//        switch_sp();
        
        TSPoint p = ts.getPoint();
        int X = (p.x - 68) * 16 / 25; //max=571, mid=286
        int Y = 274 - ((p.y - 142) * 9.0 / 25.0); //max=274, mid=137
        Input1 = X * convert1;
        Input2 = Y * convert2;
        
        if(Input1<Setpoint1-3 || Input1>Setpoint1+3 || Input2>Setpoint2+3 || Input2<Setpoint2-3) //if ball isnt close to Setpoint1
        {
            servo1.attach(SERVO1_PIN); //again attach servos
            servo2.attach(SERVO2_PIN);
            digitalWrite(LED_BUILTIN, LOW); 
            Stable=0; //change STABLE state
        }

    }//end of STABLE LOOP
}//loop end
