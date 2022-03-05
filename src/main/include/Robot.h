
#pragma once

#include "BallShooter.h"
#include "Climber.h"
#include "CanHandler.h"
#include "Collector.h"
#include "CompressorController.h"
#include "JrimmyGyro.h" // Gyro wrapper class; takes in I2C port
#include "SwerveDrive.h" // Swerve kinematics
#include "SwerveModule.h" // Swerve modules
#include "mqtt/MqttHandler.h"
#include "vision/photon.h"
#include <frc/Joystick.h> // Joystick
#include <frc/TimedRobot.h> // "Timed Robot" template
#include "Autonomous.h"

class Robot : public frc::TimedRobot {
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
        Autonomous a_Autonomous;

        frc::Joystick joystickOne; // 3D flightstick (Logitech Attack 3?)
        frc::Joystick a_XboxController;
        frc::Joystick a_buttonbox;
        
        // Swerve Drive object
        SwerveDrive a_SwerveDrive;

        BallShooter a_Shooter;
        double shooterDesiredSpeed { 0.0 };

        Collector a_Collector;

        Climber a_Climber;

        CompressorController a_CompressorController;

        // MqttHandler handler;
        // CanHandler a_canHandler;

        TargetTracker a_shooterVision;
        TargetTracker a_ballTracker;

        //stuff that autonomous needs
        BeamBreak a_BeamBreak;
};
