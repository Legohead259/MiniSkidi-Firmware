#include "micro_ros__parameters.h"

// =============================
// === PARAMETER DEFINITIONS ===
// =============================


Preferences parameterSettings;

rclc_parameter_server_t parameterService;
parameter_t wirelessMode = {
    .key=PARAM_NAME__WIRELESS_MODE, 
    .type=RCLC_PARAMETER_INT,
    .onChangePtr=wirelessModeChangeCallback
};
parameter_t reverseLeft = {
    .key=PARAM_NAME__REVERSE_LEFT, 
    .type=RCLC_PARAMETER_BOOL,
    .onChangePtr=reverseLeftChangeCallback
};
parameter_t reverseRight = {
    .key=PARAM_NAME__REVERSE_RIGHT, 
    .type=RCLC_PARAMETER_BOOL,
    .onChangePtr=reverseRightChangeCallback
};
parameter_t reverseArm = {
    .key=PARAM_NAME__REVERSE_ARM, 
    .type=RCLC_PARAMETER_BOOL,
    .onChangePtr=reverseArmChangeCallback
};
parameter_t removeMomentumLeft = {
    .key=PARAM_NAME__REMOVE_MOMENTUM_LEFT, 
    .type=RCLC_PARAMETER_BOOL,
    .onChangePtr=removeMomentumLeftChangeCallback
};
parameter_t removeMomentumRight = {
    .key=PARAM_NAME__REMOVE_MOMENTUM_RIGHT, 
    .type=RCLC_PARAMETER_BOOL,
    .onChangePtr=removeMomentumRightChangeCallback
};
parameter_t removeMomentumArm = {
    .key=PARAM_NAME__REMOVE_MOMENTUM_ARM, 
    .type=RCLC_PARAMETER_BOOL,
    .onChangePtr=removeMomentumArmChangeCallback
};

parameter_t* params[NUM_PARAMETERS] = {
    &wirelessMode,
    &reverseLeft,
    &reverseRight,
    &reverseArm,
    &removeMomentumLeft,
    &removeMomentumRight,
    &removeMomentumArm,
};

void loadPreferences() {
    parameterSettings.begin("params", false);
    
    if (!parameterSettings.isKey(PARAM_NAME__WIFI_AP_SSID)) { // Check for first run and initialize with defaults
        parameterSettings.putString(PARAM_NAME__WIFI_AP_SSID, "MiniSkidi");
        parameterSettings.putString(PARAM_NAME__WIFI_AP_PASSKEY, "");
        parameterSettings.putString(PARAM_NAME__WIFI_CLIENT_SSID, "");
        parameterSettings.putString(PARAM_NAME__WIFI_CLIENT_PASSKEY, "");
        parameterSettings.putString(PARAM_NAME__DEVICE_NAME, "MiniSkidi");
        parameterSettings.putUInt(PARAM_NAME__WIRELESS_MODE, 0);
        parameterSettings.putBool(PARAM_NAME__REVERSE_LEFT, false);
        parameterSettings.putBool(PARAM_NAME__REVERSE_RIGHT, false);
        parameterSettings.putBool(PARAM_NAME__REVERSE_ARM, false);
        parameterSettings.putBool(PARAM_NAME__REMOVE_MOMENTUM_LEFT, false);
        parameterSettings.putBool(PARAM_NAME__REMOVE_MOMENTUM_RIGHT, false);
        parameterSettings.putBool(PARAM_NAME__REMOVE_MOMENTUM_ARM, false);
    }

    // Iterate through the parameter array in memory and put loaded values there
    for (uint8_t i = 0; i < NUM_PARAMETERS; i++) {
        if (params[i]->type == RCLC_PARAMETER_BOOL) {
            params[i]->bool_value = parameterSettings.getBool(params[i]->key);
        }
        else if (params[i]->type == RCLC_PARAMETER_INT) {
            params[i]->integer_value = parameterSettings.getInt(params[i]->key);
        } 
        else if (params[i]->type == RCLC_PARAMETER_DOUBLE) {
            params[i]->double_value = parameterSettings.getDouble(params[i]->key);
        }
        else {
            params[i]->type = RCLC_PARAMETER_NOT_SET;
        }
    }
}

void initializeParameterService() {
    loadPreferences();

    // Add parameters to the server
    for (uint8_t i = 0; i < NUM_PARAMETERS; i++) { // Iterate through array of parameter structures
        if (params[i]->type == RCLC_PARAMETER_NOT_SET) { continue; } // Check for bad parameters

        RCSOFTCHECK(rclc_add_parameter(&parameterService, params[i]->key, params[i]->type));

        switch (params[i]->type) {
            case RCLC_PARAMETER_BOOL:
                RCSOFTCHECK(rclc_parameter_set_bool(
                    &parameterService, 
                    params[i]->key, 
                    params[i]->bool_value));
                break;
            case RCLC_PARAMETER_INT:
                RCSOFTCHECK(rclc_parameter_set_int(
                    &parameterService, 
                    params[i]->key, 
                    params[i]->integer_value));
                break;
            case RCLC_PARAMETER_DOUBLE:
                RCSOFTCHECK(rclc_parameter_set_double(
                    &parameterService, 
                    params[i]->key, 
                    params[i]->double_value));
                break;
        }
    }
}

bool onParameterChangedCallback(const Parameter* oldParam, const Parameter* newParam, void* context) {
    (void) context;

    if (oldParam == NULL && newParam == NULL) { // Check for callback error
        return false;
    }

    if (oldParam == NULL) { // Check for new parameter
        return false; // Do not allow new parameters
    } 
    else if (newParam == NULL) { // Check for deleting parameter
        return false; // Do not allow deleting parameters
    } 
    else { // Check for changing parameters
        for (uint8_t i=0; i<NUM_PARAMETERS; i++) { // Iterate through the parameter array
            if (strcmp(newParam->name.data, params[i]->key) == 0) { // Check if parameter is in array
                switch (newParam->value.type) { // Check parameter type and set new value accordingly
                    case RCLC_PARAMETER_BOOL:
                        params[i]->bool_value = newParam->value.bool_value;
                        break;
                    case RCLC_PARAMETER_INT:
                        params[i]->integer_value = newParam->value.integer_value;
                        break;
                    case RCLC_PARAMETER_DOUBLE:
                        params[i]->double_value = newParam->value.double_value;
                        break;
                    default:
                        break;
                }
                params[i]->onChange(); // Execute the parameters change function
                saveParam(params[i]);
                return true;
            }
        }
        return false;
    }

    return true;
}

void saveParam(parameter_t* param) {
    switch (param->type) {
        case RCLC_PARAMETER_BOOL:
            parameterSettings.putBool(param->key, param->bool_value);
            break;
        case RCLC_PARAMETER_INT:
            parameterSettings.putInt(param->key, param->integer_value);
            break;
        case RCLC_PARAMETER_DOUBLE:
            parameterSettings.putDouble(param->key, param->double_value);
            break;
        default:
            break;
    }
}


// ==================================
// === PARAMETER CHANGE CALLBACKS ===
// ==================================


void wirelessModeChangeCallback(parameter_t* param) {

}

void reverseLeftChangeCallback(parameter_t* param) {

}

void reverseRightChangeCallback(parameter_t* param) {
    
}

void reverseArmChangeCallback(parameter_t* param) {
    
}

void removeMomentumLeftChangeCallback(parameter_t* param) {
    
}

void removeMomentumRightChangeCallback(parameter_t* param) {
    
}

void removeMomentumArmChangeCallback(parameter_t* param) {
    
}