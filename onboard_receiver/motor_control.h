#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

void motor_init();
void stop();
void go_forward(int speed, int time);
void go_backward(int speed, int time);
void turn_right(int power, int time);
void turn_left(int power, int time);
void translate_right(int power, int time);
void translate_left(int power, int time);

// Smooth movement functions for navigation
void move_forward_smooth(int speed);
void move_backward_smooth(int speed);
void rotate_smooth(int speed); // positive = right, negative = left
void translate_smooth(int speed); // positive = right, negative = left
void apply_motor_control(int forward, int rotation, int translate);

#endif
