
#pragma once

#include "mqtt/MqttHandler.h"
#include <frc/TimedRobot.h> // "Timed Robot" template
#include "SwerveModule.h" // Swerve modules
#include <frc/Joystick.h> // Joystick 
#include "SwerveDrive.h" // Swerve kinematics
#include "JrimmyGyro.h" // Gyro wrapper class; takes in I2C port
#include "CanHandler.h"

class Robot : public frc::TimedRobot 
{
  public:
    Robot();
    void RobotInit();
    void RobotPeriodic();

    void DisabledInit();
    void DisabledPeriodic();

    void AutonomousInit();
    void AutonomousPeriodic();

    void TeleopInit();
    void TeleopPeriodic();

    void TestInit();
    void TestPeriodic();
  private: 
    // reads values from aduinos and resets swerve drive
    // returns true on success, false on failure
    // TODO: add button that does this
    bool resetSwerveDrive();

    JrimmyGyro a_Gyro;
    SwerveModule a_FLModule;
    SwerveModule a_FRModule;
    SwerveModule a_BLModule;
    SwerveModule a_BRModule;
     
    frc::Joystick joystickOne; // 3D flightstick (Logitech Attack 3?)
    frc::Joystick a_xBoxController; 
    frc::Joystick a_buttonbox;

    // Swerve Drive object
    SwerveDrive a_swerveyDrive; 

    //MqttHandler handler;
    CanHandler a_canHandler;
};
