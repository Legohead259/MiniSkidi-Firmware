#ifndef MICRO_ROS__PUBSUB_H
#define MICRO_ROS__PUBSUB_H

#include "uros/micro_ros__definitions.h"


// =======================
// === PUB/SUB OBJECTS ===
// =======================

extern rcl_publisher_t skidiControlPublisher;
extern rcl_timer_t skidiControlTimer;
extern rcl_subscription_t joySubscriber;
extern JoystickMsg joystickMsg;
extern BoolMsg boolMsg;


// ===========================
// === PUBLISHER CALLBACKS ===
// ===========================


void skidiControlCallback(rcl_timer_t * timer, int64_t last_call_time);


// ============================
// === SUBSCRIBER CALLBACKS ===
// ============================


void joystickCallback(const void* msgin);

#endif // MICRO_ROS__PUBSUB_H