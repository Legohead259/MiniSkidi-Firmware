#include "uros/micro_ros__pubsub.h"


// =======================
// === PUB/SUB OBJECTS ===
// =======================

rcl_publisher_t skidiControlPublisher;
rcl_timer_t skidiControlTimer;
rcl_subscription_t joySubscriber;
JoystickMsg joystickMsg;
BoolMsg boolMsg;

const unsigned long ENABLE_TIMEOUT = 500; // ms
const unsigned long SWITCH_TIMEOUT = 500; // ms

unsigned long _lastEnable = 0; // Time variable for the last time "enable" was called for software debounce
unsigned long _lastSwitch = 0; // Time variable for the last time "switch" was called for software debounce

// ===========================
// === PUBLISHER CALLBACKS ===
// ===========================


void skidiControlCallback(rcl_timer_t * timer, int64_t last_call_time) {
    RCLC_UNUSED(last_call_time);
    if (timer != NULL) {
        boolMsg.data = miniSkidiPtr->getRunning();
        RCSOFTCHECK(rcl_publish(&skidiControlPublisher, &boolMsg, NULL));
    }
}


// ============================
// === SUBSCRIBER CALLBACKS ===
// ============================


void joystickCallback(const void* msgin) {
    const JoystickMsg* msg = (const JoystickMsg*) msgin;

    // TODO: Have a mode selector between arcade drive and tank drive

    using namespace XboxController; // Choose gamepad layout. TODO: Make less magic

    // Arcade Drive

    if (msg->buttons.data[6] && (millis() - _lastEnable) > ENABLE_TIMEOUT) { // Check for enable button pressed and timeout passed
        _lastEnable = millis();
        // TODO: Make thread safe
        miniSkidiPtr->getRunning() ? miniSkidiPtr->halt() : miniSkidiPtr->start(); // Change motor enabled state
    }

    // if ((msg->axes.data[LEFT_STICK_Y] > 0+LEFT_STICK_Y_TOLERANCE_MIN) && (msg->axes.data[LEFT_STICK_Y] < 0+LEFT_STICK_Y_TOLERANCE_MAX)) { // Check for no command
    //     miniSkidiPtr->stop();
    // }
    // else if (msg->axes.data[LEFT_STICK_Y] > 0+LEFT_STICK_Y_TOLERANCE_MAX) { // Check for forward (away from user) on left joystick
    //     miniSkidiPtr->forward();
    // }
    // else if (msg->axes.data[LEFT_STICK_Y] < 0+LEFT_STICK_Y_TOLERANCE_MIN) { // Check for backward (towards user) on left joystick
    //     miniSkidiPtr->backward();
    // }

    // Static keyboard control from Foxglove Studio Joystick extension
    // TODO: Make thread safe!
    if (msg->axes.data[1] > 0) {
        miniSkidiPtr->backward(); // TODO: Set to forward when param service works and motor can be reversed
    }
    else if (msg->axes.data[1] < 0) {
        miniSkidiPtr->forward(); // TODO: Set to backward when param service works and motor can be reversed
    }
    else if (msg->axes.data[0] > 0) {
        miniSkidiPtr->turnRight(); // TODO: Set to turnLeft when param service works and motor can be reversed
    }
    else if (msg->axes.data[0] < 0) {
        miniSkidiPtr->turnLeft(); // TODO: Set to turnRight when param service works and motor can be reversed
    }
    else if (msg->buttons.data[4]) { // Raise arm
        miniSkidiPtr->raiseArm(); // TODO:
    }
    else if (msg->buttons.data[0]) { // Lower arm
        miniSkidiPtr->lowerArm();
    }
    // else if (msg ->buttons.data[2]) { // Tilt bucket down
    //     miniSkidiPtr->
    // }
    else {
        miniSkidiPtr->stop();
    }

    // if (msg->axes.data[LEFT_STICK_X] > 0+LEFT_STICK_X_TOLERANCE_MAX) { // Check for right command on left joystick
    //     miniSkidiPtr->turnRight();
    // }

    // if (msg->axes.data[LEFT_STICK_X] < 0-LEFT_STICK_X_TOLERANCE_MIN) { // Check for left command on left joystick
    //     miniSkidiPtr->turnLeft();
    // }
}