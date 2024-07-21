#ifndef MINISKIDI_H
#define MINISKIDI_H

#include <mini_skidi/motors/MotorInterface.h>

class MiniSkidi {
private:
    MotorInterface* _leftMotor;
    MotorInterface* _rightMotor;
    MotorInterface* _armMotor;

    bool _isRunning = false;

public:
    MiniSkidi(MotorInterface* left, MotorInterface* right, MotorInterface* arm);
    ~MiniSkidi();

    void run();

    void forward();
    void backward();
    void turnLeft();
    void turnRight();
    void raiseArm();
    void lowerArm();
    void start();
    void stop();
    void halt();
    void enableMotors();
    void disableMotors();

    bool getRunning() { return _isRunning; }
};

#endif // MINISKIDI_H