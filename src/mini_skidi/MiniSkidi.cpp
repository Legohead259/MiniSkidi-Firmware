#include "mini_skidi/MiniSkidi.h"

MiniSkidi::MiniSkidi(MotorInterface* left, MotorInterface* right, MotorInterface* arm) {
    _leftMotor = left;
    _rightMotor = right;
    _armMotor = arm;
}

MiniSkidi::~MiniSkidi() {
}

void MiniSkidi::forward() {
    if (!_isRunning) return;
    if (_leftMotor == nullptr || _rightMotor == nullptr) return;
    _leftMotor->forward();
    _rightMotor->forward();
}

void MiniSkidi::backward() {
    if (!_isRunning) return;
    if (_leftMotor == nullptr || _rightMotor == nullptr) return;
    _leftMotor->backward();
    _rightMotor->backward();
}

void MiniSkidi::turnLeft() {
    if (!_isRunning) return;
    if (_leftMotor == nullptr || _rightMotor == nullptr) return;
    _leftMotor->backward();
    _rightMotor->forward();
}

void MiniSkidi::turnRight() {
    if (!_isRunning) return;
    if (_leftMotor == nullptr || _rightMotor == nullptr) return;
    _leftMotor->forward();
    _rightMotor->backward();
}

void MiniSkidi::raiseArm() {
    if (!_isRunning) return;
    if (_armMotor == nullptr) return;
    _armMotor->forward();
}

void MiniSkidi::lowerArm() {
    if (!_isRunning) return;
    if (_armMotor == nullptr) return;
    _armMotor->backward();
}

void MiniSkidi::start() {
    if (_isRunning) return;
    _isRunning = true;
    enableMotors();
}

void MiniSkidi::stop() {
    if (!_isRunning) return;
    if (_leftMotor == nullptr || _rightMotor == nullptr || _armMotor == nullptr) return;
    _leftMotor->stop();
    _rightMotor->stop();
    _armMotor->stop();
}

void MiniSkidi::halt() {
    if (!_isRunning) return;
    stop();
    _isRunning = false;
}

void MiniSkidi::enableMotors() {
    if (_leftMotor == nullptr || _rightMotor == nullptr || _armMotor == nullptr) return;
    _leftMotor->enable();
    _rightMotor->enable();
    _armMotor->enable();
}

void MiniSkidi::disableMotors() {
    if (_leftMotor == nullptr || _rightMotor == nullptr || _armMotor == nullptr) return;
    _leftMotor->disable();
    _rightMotor->disable();
    _armMotor->disable();
}
