#ifndef MOTOR_INTERFACE_H
#define MOTOR_INTERFACE_H

class MotorInterface {
    public:
        virtual void forward();
        virtual void backward();
        virtual void stop();
        virtual void reversePins();
        virtual void setRemoveMomentum(bool isRemoved);
        virtual void enable();
        virtual void disable();
        virtual ~MotorInterface() {}
};

#endif // MOTOR_INTERFACE_H