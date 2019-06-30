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
#define SERVO2_PIN 3

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 900);

unsigned int noTouchCount = 0; //viariable for noTouch

// PID values
double Setpoint1, Input1, Output1;     //for X
double Setpoint2, Input2, Output2;  //for Y

// servos variables
Servo servo1; //X axis
Servo servo2; //Y axis

uint16_t homeX = 550;            // raw data value for center of touchscreen
uint16_t homeY = 550;            // raw data value for center of touchscreen             

float convert1 = 0.3;  // converts raw x values to mm. found through manual calibration
float convert2 = 0.3;   // converts raw y values to mm. found through manual calibration

int flatX = 78;
int flatY = 60;

/////TIME SAMPLE
int Ts = 50; 
unsigned long Stable=0; 
//PID const
float Kp1 = 0.3;                                                     
float Ki1 = 0.03;                                                      
float Kd1 = 0.13;

float Kp2 = 0.3;                                                       
float Ki2 = 0.03;                                                      
float Kd2 = 0.13;
long cas=0; 

//INIT PID
PID myPID1(&Input1, &Output1, &Setpoint1, Kp1, Ki1, Kd1, DIRECT);
PID myPID2(&Input2, &Output2, &Setpoint2, Kp2, Ki2, Kd2, DIRECT);
 
void setup()
{
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    
    //INIT PINS
    pinMode(LED_BUILTIN, Output1); 
    digitalWrite(LED_BUILTIN, LOW);

    Serial.begin(9600);

    //INIT OF TOUSCHSCREEN
    TSPoint p = ts.getPoint();
    Input1=120;
    Input2=65;
    
    //INIT Setpoint1
    Setpoint1 = 286 * convert1;
    Setpoint2 = 137 * convert2;
    
    //// Make plate flat
    Output1=flatX;
    Output2=flatY;
    servo1.write(Output1);
    servo2.write(Output2);
    
    //Zapnutie PID
    myPID1.SetMode(AUTOMATIC);
    myPID1.SetOutputLimits(20, 170);
    myPID2.SetMode(AUTOMATIC);
    myPID2.SetOutputLimits(20, 100);
    // TIME SAMPLE
    myPID2.SetSampleTime(Ts); 
    myPID1.SetSampleTime(Ts);  
    /////
    delay(100);
    ///
}
 
void loop()
{
    while(Stable<125) //REGULATION LOOP
    {
        TSPoint p = ts.getPoint();   //measure pressure on plate
        if (p.z > ts.pressureThreshhold) //ball is on plate
        {  
            servo1.attach(SERVO1_PIN); //connect servos
            servo2.attach(SERVO2_PIN); 
            // setDesiredPosition();  
            noTouchCount = 0;  
            TSPoint p = ts.getPoint(); // measure actual position 
            int X = (p.x - 68) * 16 / 25; //max=571, mid=286
            int Y = 274 - ((p.y - 142) * 9 / 25); //max=274, mid=137
            Input1 = X * convert1;
            Input2 = Y * convert2;
            // Input1=(p.x * convert1);  // read and convert X coordinate
            // Input2=(p.y * convert2); // read and convert Y coordinate

            if((Input1 > Setpoint1-10 && Input1 < Setpoint1+10 && Input2 > Setpoint2-10 && Input2 < Setpoint2+10))//if ball is close to Setpoint1
            {
                Stable=Stable+1; //increment STABLE
                digitalWrite(LED_BUILTIN, HIGH); 
            }
            else
            {
                digitalWrite(LED_BUILTIN, LOW); 
            }
            myPID1.Compute();  //action control X compute
            myPID2.Compute(); //   action control  Y compute   
            Serial.print(Setpoint1);   Serial.print(" <-Setpoint1, Setpoint2-> ");  Serial.println(Setpoint2);
            Serial.print("Input1: ");  Serial.print(Input1);Serial.print(",Input2: "); Serial.println(Input2); 
            Serial.print("Output1: "); Serial.print(Output1); Serial.print(",Output2: "); Serial.println(Output2);
            Serial.println("-------");
        }
        else //if there is no ball on plate
        {
            noTouchCount++; //increment no touch count

            if(noTouchCount == 75) 
            {
                noTouchCount++; 
                Output1=flatX; //make plate flat
                Output2=flatY;
                servo1.write(Output1); 
                servo2.write(Output2);
            }
            if(noTouchCount == 150) //if there is no ball on plate longer
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
    while(Stable==125)//if is stable
    { //still measure actual postiion
        // setDesiredPosition(); 
        TSPoint p = ts.getPoint();
        int X = (p.x - 68) * 16 / 25; //max=571, mid=286
        int Y = 274 - ((p.y - 142) * 9 / 25); //max=274, mid=137
        Input1 = X * convert1;
        Input2 = Y * convert2;
//        Input1=(p.x * convert1);  //read X
//        Input2=(p.y * convert2); //read Y
        if(Input1<Setpoint1-10 || Input1>Setpoint1+10 || Input2>Setpoint2+10 || Input2<Setpoint2-10  ) //if ball isnt close to Setpoint1
        {
            servo1.attach(SERVO1_PIN); //again attach servos
            servo2.attach(SERVO2_PIN);
            digitalWrite(LED_BUILTIN, LOW); 
            Stable=0; //change STABLE state
        }

    }//end of STABLE LOOP
}//loop end