#pragma once

// binary buttons on operator xbox controller
enum class OperatorButton {
    A = 0,
    B = 1,
    X = 2,
    Y = 3,
    LeftBumper = 4,
    RightBumper = 5,
    Back = 6,
    Start = 7,
    LeftJoystick = 8,
    RightJoystick = 9

};

// analog input joysticks on operator xbox controller
enum class OperatorJoystick {
    LeftXAxis = 0,
    LeftYAxis = 1,
    LeftTrigger = 2,
    RightTrigger = 3,
    RightXAxis = 4,
    RightYAxis = 5,
};

// binary buttons on driver joystick
enum class DriverBotton {
    Trigger = 0,
    ThumbButton = 1,
    Button3 = 2,
    Button4 = 3,
    Button5 = 4,
    Button6 = 5,
    Button7 = 6,
    Button8 = 7,
    Button9 = 8,
    Button10 = 9,
    Button11 = 10,
    Button12 = 11
};

// analog input joysticks on driver xbox controller
enum class DriverJoystick {
    XAxis = 0,
    YAxis = 1,
    ZAxis = 2,
    Slider = 3
};