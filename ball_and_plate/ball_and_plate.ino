#include <PID_v1.h>
#include <stdint.h>
#include <TouchScreen.h>
#include <Wire.h>
#include <Servo.h>

// Touch Screen pins
#define YP A0
#define XM A2
#define YM A1
#define XP A3

// Servo motors pins
#define SERVO1_PIN 5
#define SERVO2_PIN 6

// Constants
const float convert1 = 0.3; // converts raw x values to mm. found through manual calibration
const float convert2 = 0.3; // converts raw y values to mm. found through manual calibration

//#define MAP_X(X) (((X - 68) * 16 / 25) * convert1)
//X: max=571, mid=286
//#define MAP_Y(Y) ((274 - ((Y - 142) * 9.0 / 25.0)) * convert2)
//Y: max=274, mid=137

const int FLAT1 = 79;
const int FLAT2 = 67;

const int MAX_X = 571 * convert1;
const int MAX_Y = 274 * convert2;
//////////// IN BALAYI 274 NABOD?!?

const int BORDER_X = 30;
const int BORDER_Y = 17;

const float MID_X = 286 * convert1;
const float MID_Y = 105 * convert2;
const float OFFSET_Y = 25;
const float OFFSET_X = 60;

// PID values
double setpoint1, input1, output1; // for X
double setpoint2, input2, output2; // for Y
const int Ts = 100;

// PID coefficients
float Kp1 = 0.5;
float Ki1 = 0.0;
float Kd1 = 0.25;

float Kp2 = 0.2;
float Ki2 = 0.0;
float Kd2 = 0.1;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 900);

Servo servo1; // X axis
Servo servo2; // Y axis

PID PID1(&input1, &output1, &setpoint1, Kp1, Ki1, Kd1, DIRECT);  // Controls X axis
PID PID2(&input2, &output2, &setpoint2, Kp2, Ki2, Kd2, DIRECT); // Controls Y axis

double k = 0;
float delta = 0.01;
char x = '\0';

void switch_sp() {
//  static double k = 0;
//  static float delta = 0.01;
//  static char x = '\0';

  if (Serial.available() > 0) {
    x = Serial.read();

    switch (x)
    {
      case '1':
        setpoint1 = MID_X - OFFSET_X;
        setpoint2 = MID_Y + OFFSET_Y;
        break;
      case '2':
        setpoint1 = MID_X - OFFSET_X;
        setpoint2 = MID_Y;
        break;
      case '3':
        setpoint1 = MID_X - OFFSET_X;
        setpoint2 = MID_Y - OFFSET_Y;
        break;
      case '4':
        setpoint1 = MID_X;
        setpoint2 = MID_Y + OFFSET_Y;
        break;
      case '5':
        setpoint1 = MID_X;
        setpoint2 = MID_Y;
        break;
      case '6':
        setpoint1 = MID_X;
        setpoint2 = MID_Y - OFFSET_Y;
        break;
      case '7':
        setpoint1 = MID_X + OFFSET_X;
        setpoint2 = MID_Y + OFFSET_Y;
        break;
      case '8':
        setpoint1 = MID_X + OFFSET_X;
        setpoint2 = MID_Y;
        break;
      case '9':
        setpoint1 = MID_X + OFFSET_X;
        setpoint2 = MID_Y - OFFSET_Y;
        break;
      // case 'q':
      //   Kp1 += delta;
      //   break;
      // case 'w':
      //   Kd1 += delta;
      //   break;
      // case 'a':
      //   Kp1 -= delta;
      //   break;
      // case 's':
      //   Kd1 -= delta;
      //   break;
      // case 'p':
      //   Kp2 += delta;
      //   break;
      // case '[':
      //   Kd2 += delta;
      //   break;
      // case ';':
      //   Kp2 -= delta;
      //   break;
      // case '\'':
      //   Kd2 -= delta;
      //   break;
      default:
        break;
    }

    // PID1.SetTunings(Kp1, Ki1, Kd1);
    // PID2.SetTunings(Kp2, Ki2, Kd2);
    
    // Serial.print("Delta="); Serial.print(delta);
    // Serial.print(", Kp1="); Serial.print(Kp1);
    // Serial.print(", Kd1="); Serial.print(Kd1);
    // Serial.print(", Kp2="); Serial.print(Kp2);
    // Serial.print(", Kd2="); Serial.println(Kd2);
  }

  switch (x)
  {
    case '0': // circular motion
      setpoint1 = MID_X + 25 * cos(k);
      setpoint2 = MID_Y + 25 * sin(k);
      k = k - 0.0075;
      break;
    case '*': // infinity (lemniscate) motion
      setpoint1 = MID_X + (80 * cos(k)) / (1 + sin(k) * sin(k));
      setpoint2 = MID_Y + (70 * sin(k) * cos(k)) / (1 + sin(k) * sin(k));
      k = k + 0.005;
      break;
    default:
      break;
  }
}

void setup()
{
  // PID Settings
  PID1.SetMode(AUTOMATIC);
  PID1.SetOutputLimits(FLAT1 - 60, FLAT1 + 80);
  PID1.SetSampleTime(Ts);

  PID2.SetMode(AUTOMATIC);
  PID2.SetOutputLimits(FLAT2 - 50, FLAT2 + 50);
  PID2.SetSampleTime(Ts);

  // Attaching Servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  
  // Using LED to show when stabilized
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Using Serial to change setpoint and motion of ball
  Serial.begin(9600);
  
  // Initialize Setpoints to center of plate
  setpoint1 = MID_X;
  setpoint2 = MID_Y;
  
  // Make plate flat
  output1 = FLAT1;
  output2 = FLAT2;
  servo1.write(output1);
  servo2.write(output2);
  
  delay(100);
}

unsigned long stable = 0;
unsigned int no_touch_count = 0;

void loop()
{
//  static unsigned long stable = 0;
//  static unsigned int no_touch_count = 0;
  
  while (stable < 50) // Regulation Loop
  {
    switch_sp();
    
    TSPoint p = ts.getPoint();
    
    if (p.z > ts.pressureThreshhold) // Ball is on plate
    {
      no_touch_count = 0;

      servo1.attach(SERVO1_PIN);
      servo2.attach(SERVO2_PIN);
      
      TSPoint p = ts.getPoint();
//      int X = MAP_X(p.x);
      int X = (p.x - 68) * 16 / 25;
//      int Y = MAP_Y(p.y);
      int Y = 274 - ((p.y - 142) * 9.0 / 25.0);
      input1 = X * convert1;
      input2 = Y * convert2;
       Serial.print(X); Serial.print(" "); Serial.println(Y); // used for plotting

      if (input1 > setpoint1 - 3 && input1 < setpoint1 + 3 && input2 > setpoint2 - 3 && input2 < setpoint2 + 3)
      {
        stable++;
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else
      {
        stable = 0;
        digitalWrite(LED_BUILTIN, LOW);
      }
      
      PID1.Compute(); // Computer action control on X axis
      PID2.Compute(); // Computer action control on Y axis
    }
    else // There is no ball on plate
    {
      no_touch_count++;
      if (no_touch_count >= 300) 
      {
        output1 = FLAT1;
        output2 = FLAT2;
      }
      if (no_touch_count >= 800)
      {
        servo1.detach();
        servo2.detach();
      }
    }
    
    servo1.write(output1);
    servo2.write(output2);
  }

  servo1.detach();
  servo2.detach();

  while (stable == 50) // while stable
  {
    switch_sp();
    
    TSPoint p = ts.getPoint();
    int X = (p.x - 68) * 16 / 25;
    int Y = 274 - ((p.y - 142) * 9.0 / 25.0);
    input1 = X * convert1;
    input2 = Y * convert2;
    
    if (input1 < setpoint1 - 3 || input1 > setpoint1 + 3 || input2 > setpoint2 + 3 || input2 < setpoint2 - 3) // Ball isnt close to setpoint
    {
      servo1.attach(SERVO1_PIN); //again attach servos
      servo2.attach(SERVO2_PIN);
      digitalWrite(LED_BUILTIN, LOW);
      stable = 0;
    }
  }
}
