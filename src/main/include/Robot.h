
#pragma once

#include "Autonomous.h"
#include "BallShooter.h"
#include "CanHandler.h"
#include "Climber.h"
#include "Collector.h"
#include "CompressorController.h"
#include "JrimmyGyro.h" // Gyro wrapper class; takes in I2C port
#include "LimitSwitch.h"
#include "SwerveDrive.h" // Swerve kinematics
#include "SwerveModule.h" // Swerve modules
#include "vision/photon.h"
#include <frc/Joystick.h> // Joystick
#include <frc/TimedRobot.h> // "Timed Robot" template
#include <frc/Timer.h>

enum class DriveBackState {
    Inactive,
    Start,
    Active,
};

class Robot : public frc::TimedRobot {
    public:
        Robot();
        void RobotInit();
        void RobotPeriodic();

        void DisabledInit();
        void DisabledPeriodic();

        // called whenever the robot transitions from disabled to either autonomous, teleop, or test
        // this means it is basically called whenever the robot is enabled
        // is not called when the robot moves between autonomous, teleop, or test
        void EnabledInit();
        // called during autonomous, teleop, and test periodic
        void EnabledPeriodic();

        void AutonomousInit();
        void AutonomousPeriodic();

        void TeleopInit();
        void TeleopPeriodic();

        void TestInit();
        void TestPeriodic();

    private:
        // keeps track of when to call enabled init
        bool m_doEnabledInit { true };

        JrimmyGyro m_gyro;

        SwerveModule m_flModule;
        SwerveModule m_frModule;
        SwerveModule m_blModule;
        SwerveModule m_brModule;
        SwerveDrive m_swerveDrive;

        // if slow speed is enabled
        bool m_slowSpeed { false };

        Autonomous m_autonomous;

        frc::Joystick m_joystick; // 3D flightstick (Logitech Attack 3?)
        frc::Joystick m_xboxController;

        BallShooter m_shooter;

        Collector m_collector;

        Climber m_climber;

        CompressorController m_compressorController;

        // the max speed in meters per second of the slow speed mode
        constexpr static float SLOW_SPEED_MAX_SPEED = 1.0;
        constexpr static float SLOW_SPEED_MAX_ROT_SPEED = 90.0;
};
