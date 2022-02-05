#pragma once


#include "SwerveModule.h" // Swerve modules
#include "SwerveDrive.h" // Swerve kinematics
#include "JrimmyGyro.h" // Gyro wrapper class; takes in I2C port
#include "BallShooter.h" 

#include <frc/Joystick.h> // Joystick 
#include <frc/TimedRobot.h> // "Timed Robot" template
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/shuffleboard/ShuffleboardTab.h>
#include <frc/shuffleboard/SimpleWidget.h>
#include <frc/shuffleboard/BuiltInWidgets.h>
#include <networktables/NetworkTableEntry.h>

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
  
    JrimmyGyro a_Gyro;
    SwerveModule a_FLModule;
    SwerveModule a_FRModule;
    SwerveModule a_BLModule;
    SwerveModule a_BRModule;
     
    frc::Joystick a_Joystick;
    frc::Joystick a_XboxController; 

    // Swerve Drive object
    SwerveDrive a_SwerveDrive;

    BallShooter a_Shooter; 

    frc::ShuffleboardTab& a_mainTab;
    nt::NetworkTableEntry a_setRpm;
    nt::NetworkTableEntry a_currentRpm;

};