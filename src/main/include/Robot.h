
#pragma once

#include "Autonomous.h"
#include "BallShooter.h"
#include "CanHandler.h"
#include "Climber.h"
#include "Collector.h"
#include "CompressorController.h"
#include "JrimmyGyro.h" // Gyro wrapper class; takes in I2C port
#include "SwerveDrive.h" // Swerve kinematics
#include "SwerveModule.h" // Swerve modules
#include "vision/photon.h"
#include <frc/Joystick.h> // Joystick
#include <frc/TimedRobot.h> // "Timed Robot" template
#include <frc/Timer.h>

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
        frc::Timer a_Timer;

        // Swerve Drive object
        SwerveDrive a_SwerveDrive;

        BallShooter a_Shooter;
        double shooterDesiredSpeed { 0.0 };

        Collector a_Collector;

        Climber a_Climber;

        CompressorController a_CompressorController;

        // CanHandler a_canHandler;

        // stuff that autonomous needs
        BeamBreak a_BeamBreak;

        TargetTracker a_shooterVision;
        TargetTracker a_ballTracker;
};
