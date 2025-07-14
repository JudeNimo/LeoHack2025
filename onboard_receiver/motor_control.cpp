#include <Arduino.h>
#include "motor_control.h"

// Define motor pin constants globally in the .cpp file
const int Mot1_pwm = 2;
const int Mot1_hbridge_1 = 4;
const int Mot1_hbridge_2 = 5;
const int Mot2_pwm = 3;
const int Mot2_hbridge_1 = 6;
const int Mot2_hbridge_2 = 7;
const int Mot3_pwm = 10;
const int Mot3_hbridge_1 = 8;
const int Mot3_hbridge_2 = 9;

int global_max_speed = 200; //maximum speed which the pwoer will be normalised to
int time_normaliser = 500; //same here, value is in miliseconds

void motor_init() {
  pinMode(Mot1_pwm, OUTPUT);
  pinMode(Mot2_pwm, OUTPUT);
  pinMode(Mot3_pwm, OUTPUT);
  pinMode(Mot1_hbridge_1, OUTPUT);
  pinMode(Mot1_hbridge_2, OUTPUT);
  pinMode(Mot2_hbridge_1, OUTPUT);
  pinMode(Mot2_hbridge_2, OUTPUT);
  pinMode(Mot3_hbridge_1, OUTPUT);
  pinMode(Mot3_hbridge_2, OUTPUT);

  digitalWrite(Mot1_hbridge_1, LOW);
  digitalWrite(Mot1_hbridge_2, LOW);
  digitalWrite(Mot2_hbridge_1, LOW);
  digitalWrite(Mot2_hbridge_2, LOW);
  digitalWrite(Mot3_hbridge_1, LOW);
  digitalWrite(Mot3_hbridge_2, LOW);
}

void go_forward(int power, int time) {
  int desired_speed = (power * global_max_speed) / 9;

  digitalWrite(Mot2_hbridge_1, LOW);
  digitalWrite(Mot2_hbridge_2, HIGH);
  digitalWrite(Mot3_hbridge_1, HIGH);
  digitalWrite(Mot3_hbridge_2, LOW);

  analogWrite(Mot2_pwm, desired_speed);
  analogWrite(Mot3_pwm, desired_speed);
}

void stop(){
  analogWrite(Mot1_pwm, 0);
  analogWrite(Mot2_pwm, 0);
  analogWrite(Mot3_pwm, 0);
  digitalWrite(Mot1_hbridge_1, LOW);
  digitalWrite(Mot1_hbridge_2, LOW);
  digitalWrite(Mot2_hbridge_1, LOW);
  digitalWrite(Mot2_hbridge_2, LOW);
  digitalWrite(Mot3_hbridge_1, LOW);
  digitalWrite(Mot3_hbridge_2, LOW);

}

void go_backward(int power, int time) {
  int desired_speed = (power * global_max_speed) / 9;

  digitalWrite(Mot2_hbridge_1, HIGH);
  digitalWrite(Mot2_hbridge_2, LOW);
  digitalWrite(Mot3_hbridge_1, LOW);
  digitalWrite(Mot3_hbridge_2, HIGH);

  analogWrite(Mot2_pwm, desired_speed);
  analogWrite(Mot3_pwm, desired_speed);
}

void turn_left(int power, int time){
  int desired_speed = (power * global_max_speed) / 9;

  digitalWrite(Mot1_hbridge_1, LOW);
  digitalWrite(Mot1_hbridge_2, HIGH);
  digitalWrite(Mot2_hbridge_1, HIGH);
  digitalWrite(Mot2_hbridge_2, LOW);
  digitalWrite(Mot3_hbridge_1, HIGH);
  digitalWrite(Mot3_hbridge_2, LOW);

  analogWrite(Mot1_pwm, desired_speed);
  analogWrite(Mot2_pwm, desired_speed);
  analogWrite(Mot3_pwm, desired_speed);
}

void turn_right(int power, int time){
  int desired_speed = (power * global_max_speed) / 9;

  digitalWrite(Mot1_hbridge_1, HIGH);
  digitalWrite(Mot1_hbridge_2, LOW);
  digitalWrite(Mot2_hbridge_1, LOW);
  digitalWrite(Mot2_hbridge_2, HIGH);
  digitalWrite(Mot3_hbridge_1, LOW);
  digitalWrite(Mot3_hbridge_2, HIGH);

  analogWrite(Mot1_pwm, desired_speed);
  analogWrite(Mot2_pwm, desired_speed);
  analogWrite(Mot3_pwm, desired_speed);
}

void translate_right(int power, int time){
  int desired_speed = (power * global_max_speed) / 9;

  digitalWrite(Mot1_hbridge_1, LOW);
  digitalWrite(Mot1_hbridge_2, HIGH);
  digitalWrite(Mot2_hbridge_1, LOW);
  digitalWrite(Mot2_hbridge_2, HIGH);
  digitalWrite(Mot3_hbridge_1, LOW);
  digitalWrite(Mot3_hbridge_2, HIGH);

  analogWrite(Mot1_pwm, desired_speed);
  analogWrite(Mot2_pwm, desired_speed/2);
  analogWrite(Mot3_pwm, desired_speed/2);
}

void translate_left(int power, int time){
  int desired_speed = (power * global_max_speed) / 9;

  digitalWrite(Mot1_hbridge_1, HIGH);
  digitalWrite(Mot1_hbridge_2, LOW);
  digitalWrite(Mot2_hbridge_1, HIGH);
  digitalWrite(Mot2_hbridge_2, LOW);
  digitalWrite(Mot3_hbridge_1, HIGH);
  digitalWrite(Mot3_hbridge_2, LOW);

  analogWrite(Mot1_pwm, desired_speed);
  analogWrite(Mot2_pwm, desired_speed/2);
  analogWrite(Mot3_pwm, desired_speed/2);
}
