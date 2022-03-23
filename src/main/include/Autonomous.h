

#pragma once

// #include <frc/WPILib.h>
#include "BallShooter.h"
#include "Collector.h"
#include "SwerveDrive.h"
#include <JrimmyGyro.h>
#include <Prefs.h>
#include <frc/Joystick.h>
#include <frc/Timer.h>
#include <frc/XboxController.h>
#include <units/math.h>


enum AutoType {
    k0Ball = 0,
    kLeft1Ball = 1,
    kMiddle1Ball = 2,
    kRight1Ball = 3,
    k2Ball = 4,
    k5Ball = 5,
};

enum AutoState0 { // Encoders
    kAutoIdle0 = 0,
    kDriveAway0
};

enum AutoState1 { // Encoders
    kAutoIdle1 = 0,
    kStartShooter1,
    kWaitShooter1,
    kShoot1,
    kStartTimer1,
    kWait1,
    kDoneShooting1,
    kTaxi1
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

// states for 5 ball auto
enum class A5 {
    Idle,
    SpoolShooter,
    WaitShooter,
    Shoot1,
    Pickup2,
    Pickup3,
    GoToShoot23,
    Shoot23,
    Pickup4,
    // TODO: use ball vision to pickup the 5th one from the driver station
    WaitPickup5,
    GoToShoot45,
    Shoot45,
};



// add more vision versions later


class Autonomous {
    public:
        Autonomous(JrimmyGyro *Gyro, frc::Joystick *Joystick, frc::Joystick *XboxController, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector);
        void Init();
        // void UpdateGameData();
        void DecidePath();
        void DecidePath(int intent);

        const char *GetCurrentPath();

        void StartPathMaster();

        void PeriodicPathMaster();

        void Start0Ball();
        void Periodic0Ball();

        void StartLeft1Ball();
        void StartMiddle1Ball();
        void StartRight1Ball();
        void Periodic1Ball();

        void Start2Ball();
        void Periodic2Ball();

        void Start5Ball();
        void Periodic5Ball();

        // ------------------Sub-Routines-------------------------//

        void IDontLikeExercise(); // IDLE

        void SpoolShooter(float speed); // Spools up shooter ahead of time to improve efficiency

        // Timer System
        //      Note: you MUST have a separate case to start the timer, though WaitForTime handles stopping & resetting
        void StartTimer();
        bool WaitForTime(double time); // Wait for specified time

        bool DriveDist(double dist, double angle); // Drive a distance based off encoders

        // deploy collector and spool motoer
        void CollectorDown();
        // raise collector and stop motor
        void CollectorUp();

        // Drives in direction at speed for distance. If going straight backwards, set angle to 180, not dist as a negative
        bool DriveDirection(double dist, double angle, double speed, bool fieldOriented);

        bool TurnToAngle(float angle); // turns to a specific angle

        bool IHaveAProposal(float speed, float dir, float dist); /// drive to distance, at input speed and direction between 0-360

        bool IndexAndShoot(float speed); // Shooting a ball when the shooter is spinning fast enough


    private:
        JrimmyGyro *a_Gyro;
        frc::Joystick *a_Joystick;
        SwerveDrive *a_SwerveDrive;
        frc::Joystick *a_Xbox;
        BallShooter *a_BallShooter;
        Collector *a_Collector;

        AutoState0 a_AutoState0;
        AutoState1 a_AutoState1;
        AutoState2 a_AutoState2;
        A5 a_AutoState5;

        AutoType autoPathMaster;
        int BallsShot;
        bool prevbeam;
        bool currbeam;
        float limeangle;
        float drivestart;
        bool look;
        double waitTimeStart { 0.0 };

        // start position of robot during 5 ball auto relative to near left corner of field
        // FIXME: this is a very innacurate guess, more so than the other measurements
        constexpr static Vec2 AUTO5_START_POS { 5.52, 7.69 };
        // speed to go during 5 ball auto
        constexpr static float AUTO5_SPEED { 0.25 };
};