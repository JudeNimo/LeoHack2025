#include <Arduino.h>
#include <Servo.h>
#include "student_functions.h"

Servo Servo1, Servo2;  // Create two servo objects
int Servo1Pin = 9; //set the pins to which your servos are connected

//please note that code below is only written for one servo

void servo_init(){
  //intialise the servos
  Servo1.attach(Servo1Pin);  // Attach servo to the desired pin
}

void servo_open() {
  //sets the servo to 180 degrees
  Servo1.write(180);
  Serial.println("Turning servo 180");
  delay(50); //wait for the servo to reach the desired position
}

void servo_close(){
  //sets the servo to 0 degrees
  Servo1.write(0);
  Serial.println("Turning servo 0");
  delay(50); //wait for the servo to reach the desired position
}