#ifndef MICRO_ROS__DEFINITIONS_H
#define MICRO_ROS__DEFINITIONS_H

#include <Arduino.h>

// The micro_ros_platformio library provides the functions to communicate with ROS2
#include <micro_ros_platformio.h>

// These are core ROS2 libraries for creating nodes, publishers, and executors
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rclc_parameter/rclc_parameter.h>

// Non-Volatile Memory libraries
#ifdef ARDUINO_ARCH_ESP32
#include <Preferences.h>
#endif // ARDUINO_ARCH_ESP32

// Define macros for checking function success
#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){ while(1); }} // Blocking
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){ }} // Non-blocking

// Define macros for executing a task every interval
#define EXECUTE_EVERY_N_MS(MS, X)  do { \
    static volatile int64_t init = -1; \
    if (init == -1) { init = uxr_millis();} \
    if (uxr_millis() - init > MS) { X; init = uxr_millis();} \
} while (0)\

// Get the custom actions form the `motor_interfaces` package
#include <sensor_msgs/msg/joy.h>

// Interface type format {package}__{interface [action/srv/msg]}__{InterfaceName}_{Additional [Response/Request]}
// Redeclare types for readability
using JoystickMsg = sensor_msgs__msg__Joy;

// Instantiate ROS2 base objects
extern rcl_allocator_t allocator;
extern rclc_support_t support;
extern rcl_node_t node;
extern rclc_executor_t executor;

// Define various parameters

enum AgentState {
    WAITING_AGENT,
    AGENT_AVAILABLE,
    AGENT_CONNECTED,
    AGENT_DISCONNECTED
};

extern AgentState agentState;

#endif // MICRO_ROS__DEFINITIONS_H