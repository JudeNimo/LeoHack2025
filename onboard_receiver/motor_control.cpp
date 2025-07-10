#include <Arduino.h>
#include "motor_control.h"

// Define motor pin constants globally in the .cpp file
const int Mot1_pwm = 2;
const int Mot1_hbridge_1 = 4;
const int Mot1_hbridge_2 = 5;
const int Mot2_pwm = 3;
const int Mot2_hbridge_1 = 6;
const int Mot2_hbridge_2 = 7;

void motor_init() {
  pinMode(Mot1_pwm, OUTPUT);
  pinMode(Mot2_pwm, OUTPUT);
  pinMode(Mot1_hbridge_1, OUTPUT);
  pinMode(Mot1_hbridge_2, OUTPUT);
  pinMode(Mot2_hbridge_1, OUTPUT);
  pinMode(Mot2_hbridge_2, OUTPUT);

  digitalWrite(Mot1_hbridge_1, LOW);
  digitalWrite(Mot1_hbridge_2, LOW);
  digitalWrite(Mot2_hbridge_1, LOW);
  digitalWrite(Mot2_hbridge_2, LOW);
}

void go_forward(int power, int time) {
  int desired_speed = (power * 255) / 9;

  digitalWrite(Mot1_hbridge_1, HIGH);
  digitalWrite(Mot1_hbridge_2, LOW);
  digitalWrite(Mot2_hbridge_1, HIGH);
  digitalWrite(Mot2_hbridge_2, LOW);

  analogWrite(Mot1_pwm, desired_speed);
  analogWrite(Mot2_pwm, desired_speed);

  delay(time * 1000);

  analogWrite(Mot1_pwm, 0);
  analogWrite(Mot2_pwm, 0);

  digitalWrite(Mot1_hbridge_1, LOW);
  digitalWrite(Mot1_hbridge_2, LOW);
  digitalWrite(Mot2_hbridge_1, LOW);
  digitalWrite(Mot2_hbridge_2, LOW);
}
