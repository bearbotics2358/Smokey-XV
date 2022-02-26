

#pragma once

// #include <frc/WPILib.h>
#include "SwerveDrive.h"
#include "BallShooter.h"
#include "Collector.h"
#include <frc/Joystick.h> 
#include <frc/Timer.h>
#include <Prefs.h>
#include <JrimmyGyro.h>




enum AutoState0 { // Encoders
	kAutoIdle0 = 0,
	//kArmMove0,
	kDriveAway0
};

 enum AutoState1 { // Encoders
	kAutoIdle1 = 0,
	kTurnAround1,
	//kArmMove1,
	//kBallFind1,
	kShoot1,
    kTaxi1
    //kDriveAway1
};

enum AutoState2 { // T.O.F and Encoders
	kAutoIdle2 = 0,
	kArmMove2,
	kShoot2,
    kTurnToParallel2,
    kDriveToWall2,
    kDriveBackThroughThreeBalls2,
    kSecondShoot2
};

enum AutoState3 { // T.O.F and Encoders
	kAutoIdle3 = 0,
	kArmMove3,
	kShoot3,
    kTurnToParallel3,
    kDriveToWall3,
    kDriveBackThroughFiveBalls3,
    kDriveForwardThroughControlPanel3,
    kSecondShoot3
};

enum AutoState4 { //
	kAutoIdle4 = 0,
	kArmMove4,
	kDriveBackLonger4,
    kTurntoShoot4,
	kPrime4,
    kShootBalls4
};

enum AutoState5 { // 
	kAutoIdle5 = 0,
	kArmMove5,
	kDriveBack5,
    kTurntoShoot5,
    kShootBalls5
};



// add more vision versions later


class Autonomous
{
 public:
 
	Autonomous(JrimmyGyro *Gyro, frc::Joystick *ButtonBox, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector);
	void Init();
	//void UpdateGameData();
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

	void AutonomousStart3();
	void AutonomousPeriodic3();

	void AutonomousStart4();
	void AutonomousPeriodic4();

	void AutonomousStart5();
	void AutonomousPeriodic5();

// ------------------Sub-Routines-------------------------//

	void IDontLikeExercise(); // IDLE
	void waitplz(double anticipate); // Wait
/*
	bool MoveDaArm(double angle); // arm move to angle
	bool CheckBallPos(); // Returns true if top beam break is broken
	bool RootyTootyShooty(int count, float vel); // Shoots specified number of balls (call over and over)
*/	
	bool DriveDist(double dist, double angle); // Drive a distance based off encoders

	bool TurnTaAngle(float angle); // turns to a specific angle
	
	bool IHaveAProposal(float speed, float dir, float dist); /// drive to distance, at input speed and direction between 0-360
	
	bool TurnLime(); // Uses limelight to face target; if no target, turn until found
	
	bool GoToMcDonalds(float speed, float dir, float dist); // Driving in a direction while collecting


 private:

	
	JrimmyGyro *a_Gyro;
	frc::Joystick *a_ButtonBox;
	SwerveDrive *a_SwerveDrive;
	frc::Timer a_Anticipation;
	BallShooter *a_BallShooter;
    Collector *a_Collector;

	AutoState0 a_AutoState0;
	AutoState1 a_AutoState1;
	AutoState2 a_AutoState2;
    AutoState3 a_AutoState3;
    AutoState4 a_AutoState4;
	AutoState5 a_AutoState5;

    int autoPathMaster;
	int BallsShot;
	bool prevbeam;
	bool currbeam;
	float limeangle;
	float drivestart;
	bool look;
};