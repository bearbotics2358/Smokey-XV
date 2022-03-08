

#pragma once

// #include <frc/WPILib.h>
#include "SwerveDrive.h"
#include "BallShooter.h"
#include "Collector.h"
#include "BeamBreak.h"
#include <frc/Joystick.h> 
#include <frc/Timer.h>
#include <units/math.h>
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
	kShoot1,
	kCheckShot1,
	kDoneShooting1,
    kTaxi1
};

enum AutoState2 { // T.O.F and Encoders
	kAutoIdle2 = 0,
	kShoot2,
    kTurn2,
	kCollectDown2,
	kLoad2,
    kDriveToWall2,
    kDriveBackThroughBall2,
    kSecondShoot2,
	kSpool2,
	kCheckFirstShot2,
	kCheckSecondShot2,
	kDoneShooting2
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
 
	Autonomous(JrimmyGyro *Gyro, frc::Timer *Timer, frc::Joystick *Joystick, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector, BeamBreak *BeamBreak);
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
	bool waitplz(double time); // Wait

	void SpoolShooter(float speed); // Spools up shooter ahead of time to improve efficiency

	void ToggleCollector(); // Toggles the Collector Pistons

	bool DriveDist(double dist, double angle); // Drive a distance based off encoders

	bool BallShot(float speed); //checks if a ball has been shot by measuring RPM values

	bool TurnTaAngle(float angle); // turns to a specific angle
	
	bool IHaveAProposal(float speed, float dir, float dist); /// drive to distance, at input speed and direction between 0-360
	
	bool TurnLime(); // Uses limelight to face target; if no target, turn until found
	
	bool GoToMcDonalds(float speed, float dir, float dist); // Driving in a direction while collecting]

	bool IndexAndShoot(float speed); // Shooting a ball when the shooter is spinning fast enough


 private:

	
	JrimmyGyro *a_Gyro;
	frc::Joystick *a_Joystick;
	SwerveDrive *a_SwerveDrive;
	frc::Timer *a_Timer;
	BallShooter *a_BallShooter;
    Collector *a_Collector;
	BeamBreak *a_BeamBreak;

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