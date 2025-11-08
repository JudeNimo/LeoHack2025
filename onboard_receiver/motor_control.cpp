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
const int Mot3_hbridge_2 = 13;

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

// Smooth movement functions for proportional control
void move_forward_smooth(int speed) {
  int desired_speed = (speed * global_max_speed) / 9;
  if (desired_speed < 0) desired_speed = 0;
  if (desired_speed > global_max_speed) desired_speed = global_max_speed;

  digitalWrite(Mot2_hbridge_1, LOW);
  digitalWrite(Mot2_hbridge_2, HIGH);
  digitalWrite(Mot3_hbridge_1, HIGH);
  digitalWrite(Mot3_hbridge_2, LOW);

  analogWrite(Mot2_pwm, desired_speed);
  analogWrite(Mot3_pwm, desired_speed);
}

void move_backward_smooth(int speed) {
  int desired_speed = (speed * global_max_speed) / 9;
  if (desired_speed < 0) desired_speed = 0;
  if (desired_speed > global_max_speed) desired_speed = global_max_speed;

  digitalWrite(Mot2_hbridge_1, HIGH);
  digitalWrite(Mot2_hbridge_2, LOW);
  digitalWrite(Mot3_hbridge_1, LOW);
  digitalWrite(Mot3_hbridge_2, HIGH);

  analogWrite(Mot2_pwm, desired_speed);
  analogWrite(Mot3_pwm, desired_speed);
}

void rotate_smooth(int speed) {
  // Positive speed = rotate right, negative = rotate left
  int desired_speed = (abs(speed) * global_max_speed) / 9;
  if (desired_speed > global_max_speed) desired_speed = global_max_speed;

  if (speed > 0) {
    // Rotate right
    digitalWrite(Mot1_hbridge_1, HIGH);
    digitalWrite(Mot1_hbridge_2, LOW);
    digitalWrite(Mot2_hbridge_1, LOW);
    digitalWrite(Mot2_hbridge_2, HIGH);
    digitalWrite(Mot3_hbridge_1, LOW);
    digitalWrite(Mot3_hbridge_2, HIGH);
  } else {
    // Rotate left
    digitalWrite(Mot1_hbridge_1, LOW);
    digitalWrite(Mot1_hbridge_2, HIGH);
    digitalWrite(Mot2_hbridge_1, HIGH);
    digitalWrite(Mot2_hbridge_2, LOW);
    digitalWrite(Mot3_hbridge_1, HIGH);
    digitalWrite(Mot3_hbridge_2, LOW);
  }

  analogWrite(Mot1_pwm, desired_speed);
  analogWrite(Mot2_pwm, desired_speed);
  analogWrite(Mot3_pwm, desired_speed);
}

void translate_smooth(int speed) {
  // Positive speed = translate right, negative = translate left
  int desired_speed = (abs(speed) * global_max_speed) / 9;
  if (desired_speed > global_max_speed) desired_speed = global_max_speed;

  if (speed > 0) {
    // Translate right
    digitalWrite(Mot1_hbridge_1, LOW);
    digitalWrite(Mot1_hbridge_2, HIGH);
    digitalWrite(Mot2_hbridge_1, LOW);
    digitalWrite(Mot2_hbridge_2, HIGH);
    digitalWrite(Mot3_hbridge_1, LOW);
    digitalWrite(Mot3_hbridge_2, HIGH);
  } else {
    // Translate left
    digitalWrite(Mot1_hbridge_1, HIGH);
    digitalWrite(Mot1_hbridge_2, LOW);
    digitalWrite(Mot2_hbridge_1, HIGH);
    digitalWrite(Mot2_hbridge_2, LOW);
    digitalWrite(Mot3_hbridge_1, HIGH);
    digitalWrite(Mot3_hbridge_2, LOW);
  }

  analogWrite(Mot1_pwm, desired_speed);
  analogWrite(Mot2_pwm, desired_speed / 2);
  analogWrite(Mot3_pwm, desired_speed / 2);
}

void apply_motor_control(int forward, int rotation, int translate) {
  // Apply combined motor control based on forward, rotation, and translation components
  // This is a simplified version - for more complex control, you'd need to combine vectors
  
  // Priority: rotation > translation > forward
  if (abs(rotation) > 1) {
    rotate_smooth(rotation);
  } else if (abs(translate) > 1) {
    translate_smooth(translate);
  } else if (abs(forward) > 1) {
    if (forward > 0) {
      move_forward_smooth(forward);
    } else {
      move_backward_smooth(-forward);
    }
  } else {
    stop();
  }
}
