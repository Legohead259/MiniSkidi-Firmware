#ifndef MICRO_ROS__JOYSTICK_MAP_H
#define MICRO_ROS__JOYSTICK_MAP_H

namespace XboxController {
    const double LEFT_STICK_X_TOLERANCE_MIN = -0.04;
    const double LEFT_STICK_X_TOLERANCE_MAX = 0.04;
    const double LEFT_STICK_Y_TOLERANCE_MIN = -0.04;
    const double LEFT_STICK_Y_TOLERANCE_MAX = 0.04;

    const double RIGHT_STICK_X_TOLERANCE_MIN = -0.04;
    const double RIGHT_STICK_X_TOLERANCE_MAX = 0.04;
    const double RIGHT_STICK_Y_TOLERANCE_MIN = -0.04;
    const double RIGHT_STICK_Y_TOLERANCE_MAX = 0.04;

    enum Axes {
        LEFT_STICK_X,
        LEFT_STICK_Y,
        RIGHT_STICK_X,
        RIGHT_STICK_Y
    };

    enum Buttons {
        BUTTON_A,
        BUTTON_B,
        BUTTON_X,
        BUTTON_Y,
        LEFT_SHOULDER,
        RIGHT_SHOULDER,
        LEFT_TRIGGER,
        RIGHT_TRIGGER,
        SELECT,
        START,
        LEFT_STICK,
        RIGHT_STICK,
        D_PAD_UP,
        D_PAD_DOWN,
        D_PAD_RIGHT,
        D_PAD_LEFT
    };
}

#endif // MICRO_ROS__JOYSTICK_MAP_H