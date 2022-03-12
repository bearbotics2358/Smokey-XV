
#pragma once // only add this code once; saves space by removing redundancy

#include <units/angle.h>
#include <units/length.h>

// Declare constants such as CAN IDs here

// uncomment to enable the new swerve
//#define NEW_SWERVE

/*======== CLIMBER CONSTANTS ========*/

#define CLIMBER_MOTOR_ID 24
#define CLIMBER_PUSH_SOLENOID_MODULE 10
#define CLIMBER_PULL_SOLENOID_MODULE 11
#define CLIMBER_MOTOR_SPEED 1000 // the desired speed for the arm to rise (mm/s)
#define CLIMBER_MM_PER_TICK 0.026 // how many mm the climber arm rises every motor tick
#define CLIMBER_TICKS_PER_MM 3.8347 // how many ticks for every mm the climber arm rises

/*======== SHOOTER CONSTANTS ========*/

#define LEFT_SHOOTER_ID 26
#define RIGHT_SHOOTER_ID 27
#define SHOOTER_MOTOR_RPM 220
#define SHOOTER_SPEED 2000
#define SHOOT_FROM_WALL 1000

/*======== COLLECTOR CONSTANTS ========*/

#define COLLECTOR_MOTOR_ID 21
#define INDEXER_MOTOR_ID 22
#define SOLENOID_ID 10
#define COLLECTOR_MOTOR_PERCENT_POWER 0.5
// NOTE: this is not actully the rpm it will go because of the gear ratio, but it goes the speed we want
#define INDEXER_MOTOR_PERCENT_POWER 0.2
#define COLLECTOR_PUSH_SOLENOID_MODULE 8
#define COLLECTOR_PULL_SOLENOID_MODULE 9


/*====== MOTOR CONTROLLER IDS ======*/
#define FL_DRIVE_ID 1
#define FL_STEER_ID 2

#define FR_DRIVE_ID 3
#define FR_STEER_ID 8

#define BL_DRIVE_ID 5
#define BL_STEER_ID 6

#define BR_DRIVE_ID 7
#define BR_STEER_ID 4


/*======= ENCODER CONSTANTS =======*/

// falcon encoder ticks per 1 revolution
#define FALCON_UNITS_PER_REV 2048

// swerve drive absolute encoder analog ports
#define FL_SWERVE_ABS_ENC_PORT 0
#define FR_SWERVE_ABS_ENC_PORT 2
#define BL_SWERVE_ABS_ENC_PORT 1
#define BR_SWERVE_ABS_ENC_PORT 3

// min and max voltage of absolute encoders on swerve drives
#define FL_SWERVE_ABS_ENC_MIN_VOLTS 0.014076
#define FL_SWERVE_ABS_ENC_MAX_VOLTS 4.952392

#define FR_SWERVE_ABS_ENC_MIN_VOLTS 0.037842
#define FR_SWERVE_ABS_ENC_MAX_VOLTS 4.962158

#define BL_SWERVE_ABS_ENC_MIN_VOLTS 0.004883
#define BL_SWERVE_ABS_ENC_MAX_VOLTS 4.641113

#define BR_SWERVE_ABS_ENC_MIN_VOLTS 0.010986
#define BR_SWERVE_ABS_ENC_MAX_VOLTS 4.963378

// offset from 0 of the absolute encders
//#define FL_SWERVE_ABS_ENC_OFFSET 16.24
//#define FR_SWERVE_ABS_ENC_OFFSET 85.76
//#define BL_SWERVE_ABS_ENC_OFFSET 0.22
//#define BR_SWERVE_ABS_ENC_OFFSET 179.77
#define FL_SWERVE_ABS_ENC_OFFSET 344.25
#define FR_SWERVE_ABS_ENC_OFFSET 288.92
#define BL_SWERVE_ABS_ENC_OFFSET 274.73
#define BR_SWERVE_ABS_ENC_OFFSET 359.83


/* ========== Joystick Ports ========= */
#define JOYSTICK_PORT 1
#define JOYSTICK_DEADZONE 0.15

#define XBOX_CONTROLLER 5

#define TICKS_STEERING 18.0 // roughly 18 "position" units per steering rotation

/* ====== ASCII ART ======= */
//   <====[-<('-'<)

#define LIGHT_RING_CONTROLLER_ADDRESS 0x4
/* ============ GEAR RATIOS ======== */
// drive motor -> wheel = 10:1 (10 drive rotations for one wheel rotation)
// radius of wheel = 2 inches
// circumfrence = 4 Pi inches
// 10 ticks = 4 Pi inches
#define INCHES_PER_TICK (2.0 / 5) * M_PI // inches

// TODO: figure out what this actually is
// ratio is drive motor rotations / wheel rotations
#define SWERVE_DRIVE_MOTOR_GEAR_RATIO (18.0 / 1.0)

// FIXME: this is wrong, must be in meters
#define WHEEL_DIAMETER 4.0

/* ============= AUTO VALUES 4 Jason yoyoyo =========== */
#define TO_TRENCH_FROM_LINE 65 // inches
#define TOF_TO_WALL 27.0 // inches
#define LINE_TO_BALL_FAR 192.66 // inches
#define TO_BALL2_FROM_LINE 160 // inches


#define VOLTS_MAX 1.65 // Subject to change cause movement
#define VOLTS_MIN 2.52 // Subject to change cause movement
#define ANGLE_MAX 90
#define ANGLE_MIN 24

#define AUTO_START_BALL_NUM 3 // KNOWN FACT ITS WHAT WE START WITH!!!!!

#define ARM_DEFAULT_POSITION 89 // degrees???? GOOD!
#define AUTO_ARM_FINAL_POS 35 // degrees???? GOOD!
#define AUTO_SHOOT_VELOCITY 463 // RPM, GOOOD!
#define AUTO_SHOOT_VELOCITY5BALL 440 // could change
#define AUTO_FEED_VAL 1000 // wowowowow
#define AUTO_ANGLE_DRIVESTRAIGHT 180 // Degrees
#define AUTO_DRIVE_SPEED .4




/*----------------------------------------------*/

#define SHOOT_VOLTS .7
#define COLLECT_SPEED .5
#define FEED_SPEED .23

#define SHOOT_VELOCITY 462.5
#define DRIVE_VELOCITY 500
#define FEED_VELOCITY 1200 // velocity of the feeder stuff

#define SHOOT_1 41
#define SHOOT_2 42
#define FEED_1 43
#define FEED_2 44
#define COLLECT 45
#define PIVOT 46
#define CLIMBER 47

// int shoot1, int shoot2, int feed1, int feed2, int collect, int pivot


/* ============= MqttHandler ============= */

#define SEND_BUF_LEN 2048
#define RECV_BUF_LEN 2048


/* ============= CanHandler ============= */

#define LEFT_ARDUINO_CAN_ID 1
#define RIGHT_ARDUINO_CAN_ID 1

#define LEFT_ARDUINO_API_ID 2
#define RIGHT_ARDUINO_API_ID 3

#define FL_SWERVE_DATA_ID 0
#define BL_SWERVE_DATA_ID 1
#define FR_SWERVE_DATA_ID 2
#define BR_SWERVE_DATA_ID 3


/* ============= Vision ============= */

// the height of the camare used to track the target for shooting
constexpr units::length::meter_t TARGET_CAMERA_HEIGHT = units::length::meter_t(1.0);

// pitch of vision camera that tracks the target, positive is up
constexpr units::angle::radian_t TARGET_CAMERA_PITCH = units::angle::radian_t(1.0);

// the height of the target we are shooting at
constexpr units::length::meter_t TARGET_HEIGHT = units::length::meter_t(1.0);

// pitch of target
constexpr units::angle::radian_t TARGET_PITCH = units::angle::radian_t(0.0);

// mdns name of camera
#define SHOOTER_CAMERA_NAME "photonvision"
#define BALL_CAMERA_NAME "temp"