

#pragma once

// #include <frc/WPILib.h>
#include "BallShooter.h"
#include "Collector.h"
#include "SwerveDrive.h"
#include <JrimmyGyro.h>
#include <Prefs.h>
#include <frc/Joystick.h>
#include <frc/XboxController.h>
#include <frc/Timer.h>
#include <units/math.h>




enum AutoState0 { // Encoders
    kAutoIdle0 = 0,
    kDriveAway0
};

enum AutoState1 { // Encoders
    kAutoIdle1 = 0,
    kShoot1,
    kStartTimer1,
    kWait1,
    kDoneShooting1,
    kTaxi1
};

enum AutoState1_1 { // Encoders
    kAutoIdle1_1 = 0,
    kShoot1_1,
    kStartTimer1_1,
    kWait1_1,
    kDoneShooting1_1,
    kTaxi1_1
};

enum AutoState2 { // T.O.F and Encoders
    kAutoIdle2 = 0,
    kDriveBackThroughBall2,
    kTurn2,
    kDriveToWall2,
    kShoot2,
    kWait2
    /*
    kSecondShoot2,
    kCheckSecondShot2,
    kWait2_2*/
};

enum AutoState2_1 { // T.O.F and Encoders
    kAutoIdle2_1 = 0,
    kDriveBackThroughBall2_1,
    kTurn2_1,
    kDriveToWall2_1,
    kShoot2_1,
    kWait2_1
    /*
    kSecondShoot2,
    kCheckSecondShot2,
    kWait2_2*/
};


// add more vision versions later


class Autonomous {
    public:
        Autonomous(JrimmyGyro *Gyro, frc::Timer *Timer, frc::Joystick *Joystick, frc::Joystick *XboxController, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector);
        void Init();
        // void UpdateGameData();
        void DecidePath();
        void DecidePath(int intent);

        const char * GetCurrentPath();

        void StartPathMaster();
        void StartPathMaster(int path);

        void PeriodicPathMaster();
        void PeriodicPathMaster(int path);

        void AutonomousStart0();
        void AutonomousPeriodic0();

        void AutonomousStart1();
        void AutonomousPeriodic1();

        void AutonomousStart1_1();
        void AutonomousPeriodic1_1();

        void AutonomousStart2();
        void AutonomousPeriodic2();
        
        void AutonomousStart2_1();
        void AutonomousPeriodic2_1();

        // ------------------Sub-Routines-------------------------//

        void IDontLikeExercise(); // IDLE

        void SpoolShooter(float speed); // Spools up shooter ahead of time to improve efficiency

        // Timer System
        //      Note: you MUST have a separate case to start the timer, though WaitForTime handles stopping & resetting
        void StartTimer();
        bool WaitForTime(double time); // Wait for specified time

        bool DriveDist(double dist, double angle); // Drive a distance based off encoders

        //changes solenoid implementation because of change in logic (down is deployed, up is retracted)
        void CollectorDown();
        void CollectorUp();
        //activates or deactivates collector motor
        void CollectorOn();
        void CollectorOff();

// Drives in direction at speed for distance. If going straight backwards, set angle to 180, not dist as a negative
        bool DriveDirection(double dist, double angle, double speed, bool fieldOriented); 
    
        bool TurnToAngle(float angle); // turns to a specific angle

        bool IHaveAProposal(float speed, float dir, float dist); /// drive to distance, at input speed and direction between 0-360

        bool IndexAndShoot(float speed); // Shooting a ball when the shooter is spinning fast enough


    private:
        JrimmyGyro *a_Gyro;
        frc::Timer *a_Timer;
        frc::Joystick *a_Joystick;
        frc::Joystick *a_Xbox;
        SwerveDrive *a_SwerveDrive;
        BallShooter *a_BallShooter;
        Collector *a_Collector;

        AutoState0 a_AutoState0;
        AutoState1 a_AutoState1;
        AutoState1_1 a_AutoState1_1;
        AutoState2 a_AutoState2;
        AutoState2_1 a_AutoState2_1;

        int autoPathMaster;
        int BallsShot;
        bool prevbeam;
        bool currbeam;
        float limeangle;
        float drivestart;
        bool look;
};