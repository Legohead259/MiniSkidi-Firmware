#include "micro_ros__motors.h"

MotorInterface* _leftMotor;
MotorInterface* _rightMotor;
MotorInterface* _armMotor;

void attachMotors(MotorInterface* left, MotorInterface* right, MotorInterface* arm) {
    _leftMotor = left;
    _rightMotor = right;
    _armMotor = arm;
}

void forward() {
    if (_leftMotor == nullptr || _rightMotor == nullptr) return;
    _leftMotor->forward();
    _rightMotor->forward();
}

void backward() {
    if (_leftMotor == nullptr || _rightMotor == nullptr) return;
    _leftMotor->backward();
    _rightMotor->backward();
}

void turnLeft() {
    if (_leftMotor == nullptr || _rightMotor == nullptr) return;
    _leftMotor->backward();
    _rightMotor->forward();
}

void turnRight() {
    if (_leftMotor == nullptr || _rightMotor == nullptr) return;
    _leftMotor->forward();
    _rightMotor->backward();
}

void raiseArm() {
    if (_armMotor == nullptr) return;
    _armMotor->forward();
}

void lowerArm() {
    if (_armMotor == nullptr) return;
    _armMotor->backward();
}

void stop() {
    if (_leftMotor == nullptr || _rightMotor == nullptr || _armMotor == nullptr) return;
    _leftMotor->stop();
    _rightMotor->stop();
    _armMotor->stop();
}

void enableMotors() {
    if (_leftMotor == nullptr || _rightMotor == nullptr || _armMotor == nullptr) return;
    _leftMotor->enable();
    _rightMotor->enable();
    _armMotor->enable();
}

void disableMotors() {
    if (_leftMotor == nullptr || _rightMotor == nullptr || _armMotor == nullptr) return;
    _leftMotor->disable();
    _rightMotor->disable();
    _armMotor->disable();
}
