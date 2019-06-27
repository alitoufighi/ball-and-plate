////#define clockwiseButton A0
////#define antiClockwiseButton A1
////#include <SoftwareSerial.h>
//#include <Servo.h>
//#define servoPin A3
//
////SoftwareSerial vSerial(12, 13);
//Servo servo;  // our servo motor has range -180 and +180.
//
//int pos;
//
//void setup() {
//  pos = 0;
////  vSerial.begin(9600);
//  servo.attach(servoPin);
////  pinMode(clockwiseButton, INPUT);
////  pinMode(antiClockwiseButton, INPUT);
//  servo.write(pos);
//}
//
//void loop() {
////  if(digitalRead(clockwiseButton) == HIGH){
////    vSerial.println("Clockwise");
////    pos += 1;
////    pos = pos % 180;
////    servo.write(pos);
////  }
////  while(digitalRead(clockwiseButton) == HIGH);
////  
////  if(digitalRead(antiClockwiseButton) == HIGH){
////    vSerial.println("Anti Clockwise");
////    pos -= 1;
////    pos = pos % 180;
////    servo.write(pos);
////  }
////  while(digitalRead(antiClockwiseButton) == HIGH);
//
//    pos += 5;
//    pos = pos % 180;
//    servo.write(pos);
//  
//  delay(2000);
//}





//#include <Servo.h>
//
//Servo myservo;  // create servo object to control a servo
//// twelve servo objects can be created on most boards
//
//int pos = 0;    // variable to store the servo position
//
//void setup() {
//  myservo.attach(3);  // attaches the servo on pin 9 to the servo object
//}
//
//void loop() {
//  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(1500);                       // waits 15ms for the servo to reach the position
//  }
//  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(1500);                       // waits 15ms for the servo to reach the position
//  }
//}

#include <Servo.h>

Servo servo1;
//Servo servo2;

//int val;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
servo1.attach(3);
//servo2.attach(5);
//val1 = 0;
//val2 = 0;

servo1.write(0);
//pinMode(LED_BUILTIN, OUTPUT);
//servo2.write(0);
//digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
   // put your main code here, to run repeatedly:

//  int i;
//  int i;
//  for (i=0;i<180;i+=5){
//  int i;
//  for(i=90;i>=0;--i){
//    servo1.write(i);
//    Serial.println(i);
//    delay(1000);
//  }
//    servo1.write(0);
//    Serial.println(0);
//    digitalWrite(LED_BUILTIN, HIGH);
//    delay(2000);
////  }
////  for(i=180;i>=0;i-=5){
//    servo1.write(180);
//    Serial.println(180);
//    digitalWrite(LED_BUILTIN, LOW);
//    delay(2000);
////  }
//  servo1.write(200);
//  Serial.println("-45\n");
//  delay(5000);
//  servo1.write(0);
//  Serial.println("0\n");
//  delay(5000);
//  servo1.write(45);
//  Serial.println("45\n");
//  delay(5000);
//  servo1.write(90);
//  Serial.println("90\n");
//  delay(5000);
//  servo1.write(135);
//  Serial.println("135\n");
//  delay(5000);
//  servo1.write(180);
//  Serial.println("180\n");
//  delay(5000);
//  for(i=0; i<90;i+=5){
//    servo1.write(i);
//    delay(500);
//  }
//  for(i=90; i>=0;i-=5){
//    servo1.write(i);
//    delay(500);
//  }
//servo1.write(0);
//servo2.write(0);
//delay(2000);
//servo1.write(90);
//servo2.write(90);
//delay(2000);
//servo1.write(180);
//servo2.write(180);
//delay(2000);
}
