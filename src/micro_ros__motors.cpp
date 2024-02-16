#include "micro_ros__motors.h"

MotorInterface* leftMotor;
MotorInterface* rightMotor;
MotorInterface* armMotor;

void attachMotors(MotorInterface* left, MotorInterface* right, MotorInterface* arm) {
    leftMotor = left;
    rightMotor = right;
    armMotor = arm;
}

void forward() {
    if (leftMotor == nullptr || rightMotor == nullptr) return;
    leftMotor->forward();
    rightMotor->forward();
}

void backward() {
    if (leftMotor == nullptr || rightMotor == nullptr) return;
    leftMotor->backward();
    rightMotor->backward();
}

void turnLeft() {
    if (leftMotor == nullptr || rightMotor == nullptr) return;
    leftMotor->backward();
    rightMotor->forward();
}

void turnRight() {
    if (leftMotor == nullptr || rightMotor == nullptr) return;
    leftMotor->forward();
    rightMotor->backward();
}

void raiseArm() {
    if (armMotor == nullptr) return;
    armMotor->forward();
}

void lowerArm() {
    if (armMotor == nullptr) return;
    armMotor->backward();
}

void stop() {
    if (leftMotor == nullptr || rightMotor == nullptr || armMotor == nullptr) return;
    leftMotor->stop();
    rightMotor->stop();
    armMotor->stop();
}

void enableMotors() {
    if (leftMotor == nullptr || rightMotor == nullptr || armMotor == nullptr) return;
    leftMotor->enable();
    rightMotor->enable();
    armMotor->enable();
}

void disableMotors() {
    if (leftMotor == nullptr || rightMotor == nullptr || armMotor == nullptr) return;
    leftMotor->disable();
    rightMotor->disable();
    armMotor->disable();
}
