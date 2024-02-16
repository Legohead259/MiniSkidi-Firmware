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

#include "micro_ros__mini_skidi.h"

void setup()  {
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    
    xTaskCreatePinnedToCore(
        microROSTaskCallback, /* Function to implement the task */
        "microROS", /* Name of the task */
        25000,  /* Stack size in words */
        NULL,  /* Task input parameter */
        0,  /* Priority of the task */
        &MicroROSTask,  /* Task handle. */
        0); /* Core where the task should run */

    set_microros_serial_transports(Serial); // Configure Micro-ROS library to use Arduino serial
}

void loop() {
    
}
