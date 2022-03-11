

#pragma once

// #include <frc/WPILib.h>
#include "BallShooter.h"
#include "BeamBreak.h"
#include "Collector.h"
#include "SwerveDrive.h"
#include <JrimmyGyro.h>
#include <Prefs.h>
#include <frc/Joystick.h>
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

enum AutoState2 { // T.O.F and Encoders
    kAutoIdle2 = 0,
    kCollectDown2,
    kDriveBackThroughBall2,
    kLoad2,
    kTurn2,
    kSpool2,
    kDriveToWall2,
    kShoot2,
    kWait1_2,
    kSecondShoot2,
    kCheckSecondShot2,
    kWait2_2
};



// add more vision versions later


class Autonomous {
    public:
        Autonomous(JrimmyGyro *Gyro, frc::Timer *Timer, frc::Joystick *Joystick, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector, BeamBreak *BeamBreak);
        void Init();
        // void UpdateGameData();
        void DecidePath();
        void DecidePath(int intent);

        int GetCurrentPath();

        void StartPathMaster();
        void StartPathMaster(int path);

        void PeriodicPathMaster();
        void PeriodicPathMaster(int path);

        void AutonomousStart0();
        void AutonomousPeriodic0();

        void AutonomousStart1();
        void AutonomousPeriodic1();

        void AutonomousStart2();
        void AutonomousPeriodic2();

        // ------------------Sub-Routines-------------------------//

        void IDontLikeExercise(); // IDLE

        void SpoolShooter(float speed); // Spools up shooter ahead of time to improve efficiency

        //Timer System
        //     Note: you MUST have a separate case to start the timer, though WaitForTime handles stopping & resetting
        void StartTimer();
		bool WaitForTime(double time); // Wait
		void ToggleCollector();

        bool DriveDist(double dist, double angle); // Drive a distance based off encoders

        bool DriveDirection(float speed, float angle); //implements driveDistance from SwerveDrive (drives distance @ angle)

        bool BallShot(float speed); // checks if a ball has been shot by measuring RPM values

        bool TurnToAngle(float angle); // turns to a specific angle

        bool IHaveAProposal(float speed, float dir, float dist); /// drive to distance, at input speed and direction between 0-360

        bool DriveWhileCollecting(double dist, double angle); // Driving in a direction while collecting

        bool IndexAndShoot(float speed); // Shooting a ball when the shooter is spinning fast enough


    private:
        JrimmyGyro *a_Gyro;
        frc::Timer *a_Timer;
        frc::Joystick *a_Joystick;
        SwerveDrive *a_SwerveDrive;
        BallShooter *a_BallShooter;
        Collector *a_Collector;
        BeamBreak *a_BeamBreak;

        AutoState0 a_AutoState0;
        AutoState1 a_AutoState1;
        AutoState2 a_AutoState2;

        int autoPathMaster;
        int BallsShot;
        bool prevbeam;
        bool currbeam;
        float limeangle;
        float drivestart;
        bool look;
};