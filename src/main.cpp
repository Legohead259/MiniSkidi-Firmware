/**
 * @file main.cpp
 *
 * @author Professor Boots (original)
 * @author Braidan Duffy (fork)
 * @brief 
 * @version 2.0.0
 * @date 2023-11-12
 * 
 * @copyright Copyright (c) 2023
 * 
 * @
 */

#include <Arduino.h>

#include <uros/micro_ros__mini_skidi.h>
#include <mini_skidi/MiniSkidi.h>
#include <mini_skidi/motors/drivers/DRV8833.h>

TaskHandle_t MiniSkidiTask;
MiniSkidi skidi((MotorInterface*) &leftMotor, (MotorInterface*) &rightMotor, (MotorInterface*) &armMotor);

void miniSkidiTaskCallback(void* parameters) {
    // skidi.run();
}

void setup()  {
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    
    xTaskCreatePinnedToCore(
        microROSTaskCallback,   /* Function to implement the task */
        "microROS::main",       /* Name of the task */
        25000,                  /* Stack size in words */
        &skidi,                 /* Task input parameter */
        0,                      /* Priority of the task */
        &MicroROSTask,          /* Task handle. */
        0);                     /* Core where the task should run */

    // xTaskCreatePinnedToCore(
    //     miniSkidiTaskCallback,  /* Function to implement the task */
    //     "miniSkidi::main",      /* Name of the task */
    //     25000,                  /* Stack size in words */
    //     NULL,                   /* Task input parameter */
    //     0,                      /* Priority of the task */
    //     &MiniSkidiTask,         /* Task handle. */
    //     1);                     /* Core where the task should run */

    set_microros_serial_transports(Serial); // Configure Micro-ROS library to use Arduino serial
}

void loop() {
    
}
