
#pragma once

#include "mqtt/MqttHandler.h"
#include <frc/TimedRobot.h> // "Timed Robot" template
#include "SwerveModule.h" // Swerve modules
#include <frc/Joystick.h> // Joystick 
#include "SwerveDrive.h" // Swerve kinematics
#include "BallShooter.h"
#include "Collector.h"
#include "JrimmyGyro.h" // Gyro wrapper class; takes in I2C port
#include "CanHandler.h"
#include "vision/photon.h"
#include "CompressorController.h"

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
     
    frc::Joystick joystickOne; // 3D flightstick (Logitech Attack 3?)
    frc::Joystick a_XboxController; 
    frc::Joystick a_buttonbox;

    // Swerve Drive object
    SwerveDrive a_SwerveDrive;

    BallShooter a_Shooter;
    double shooterDesiredSpeed { 0.0 };
  
    Collector a_Collector;

    CompressorController a_CompressorController;

    //MqttHandler handler;
    //CanHandler a_canHandler;

    TargetTracker a_shooterVision;
    TargetTracker a_ballTracker;
};
