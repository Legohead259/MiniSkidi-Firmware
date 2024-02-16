#ifndef DRV8833_H
#define DRV8833_H

#include <Arduino.h>
#include "MotorInterface.h"

class DRV8833 : MotorInterface {
    public:
        DRV8833(uint8_t in1, uint8_t in2, bool removeMomentum=false);

        void forward() override;
        void backward() override;
        void stop() override;
        void reversePins() override;

        void enable() override { isEnabled = true; }
        void disable() override { isEnabled = false; }

    private:
        uint8_t pinIn1;
        uint8_t pinIn2;
        bool removeMomentum;
        bool isEnabled;

        void _removeMomentum();
        void _setBridgePins(bool in1, bool in2);
};

extern DRV8833 rightMotor;
extern DRV8833 leftMotor;
extern DRV8833 armMotor;
#endif // DRV8833_H