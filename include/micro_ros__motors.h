#ifndef MICRO_ROS__MOTORS_H
#define MICRO_ROS__MOTORS_H

#include "micro_ros__definitions.h"

extern MotorInterface* leftMotor;
extern MotorInterface* rightMotor;
extern MotorInterface* armMotor;

void attachMotors(MotorInterface* left, MotorInterface* right, MotorInterface* arm);
void forward();
void backward();
void turnLeft();
void turnRight();
void raiseArm();
void lowerArm();
void stop();
void enableMotors();
void disableMotors();

#endif // MICRO_ROS__MOTORS_H