#ifndef MICRO_ROS__PARAMETERS_H
#define MICRO_ROS__PARAMETERS_H

#include "micro_ros__definitions.h"

// ============================
// === PARAMETER DEFINTIONS ===
// ============================


#ifdef ARDUINO_ARCH_ESP32
extern Preferences parameterSettings;
#endif // ARDUINO_ARCH_ESP32

// NOTE: Due to restrictions within the Preferences library, parameter names are limited to 16 characters!

#define PARAM_NAME__WIFI_AP_SSID "ap_ssid"
#define PARAM_NAME__WIFI_AP_PASSKEY "ap_passkey"
#define PARAM_NAME__WIFI_CLIENT_SSID "client_ssid"
#define PARAM_NAME__WIFI_CLIENT_PASSKEY "client_passkey"
#define PARAM_NAME__DEVICE_NAME "device_name"
#define PARAM_NAME__WIRELESS_MODE "wireless_mode"
#define PARAM_NAME__REVERSE_LEFT "reverse_left"
#define PARAM_NAME__REVERSE_RIGHT "reverse_right"
#define PARAM_NAME__REVERSE_ARM "reverse_arm"
#define PARAM_NAME__REMOVE_MOMENTUM_LEFT "no_momentum_lft"
#define PARAM_NAME__REMOVE_MOMENTUM_RIGHT "no_momentum_rgt"
#define PARAM_NAME__REMOVE_MOMENTUM_ARM "no_momentum_arm"

#define NUM_PARAMETERS 7

struct parameter_t; // Forward declare the struct

typedef std::function<void(parameter_t*)> ParameterChangeHandler;

typedef struct parameter_t {
    const char* key;
    rclc_parameter_type_t type;
    bool bool_value;
    int64_t integer_value;
    double double_value;
    ParameterChangeHandler onChangePtr;

    void onChange() { if (onChangePtr != nullptr) onChangePtr(this); }
} parameter_t;

extern parameter_t wireless_mode;
extern parameter_t reverse_left;
extern parameter_t reverse_right;
extern parameter_t reverse_arm;
extern parameter_t no_momentum_left;
extern parameter_t no_momentum_right;
extern parameter_t no_momentum_arm;
extern parameter_t* params[NUM_PARAMETERS];


// Initialize parameter server options
const rclc_parameter_options_t parameterServiceOpts = {
    .notify_changed_over_dds = true,        // Publish parameter events to other ROS 2 nodes as well.
    .max_params = NUM_PARAMETERS,           // Maximum number of parameters allowed on the rclc_parameter_server_t object.
    .allow_undeclared_parameters = false,   // Allows creation of parameters from external parameter clients.
    .low_mem_mode = false                   // Reduces the memory used by the parameter server, applies constraints
};

void initializeParameterService();

void saveParam(parameter_t* param);


// ==================================
// === PARAMETER CHANGE CALLBACKS ===
// ==================================


void wirelessModeChangeCallback(parameter_t* param);
void reverseLeftChangeCallback(parameter_t* param);
void reverseRightChangeCallback(parameter_t* param);
void reverseArmChangeCallback(parameter_t* param);
void removeMomentumLeftChangeCallback(parameter_t* param);
void removeMomentumRightChangeCallback(parameter_t* param);
void removeMomentumArmChangeCallback(parameter_t* param);

#endif // MICRO_ROS__PARAMETERS_H