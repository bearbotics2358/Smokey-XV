#include "Autonomous.h"
#include <math.h>


Autonomous::Autonomous(JrimmyGyro *Gyro, frc::Timer *Timer, frc::Joystick *Joystick, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector):
a_Gyro(Gyro),
a_Timer(Timer),
a_Joystick(Joystick),
a_SwerveDrive(SwerveDrive),
a_BallShooter(BallShooter),
a_Collector(Collector),
a_AutoState0(kAutoIdle0),
a_AutoState1(kAutoIdle1),
a_AutoState2(kAutoIdle2)

{

    autoPathMaster = 1;
    BallsShot = 0;
    prevbeam = false;
    currbeam = true;
    limeangle = 0.0;
    drivestart = 0.0;
    look = true;
}

void Autonomous::Init() {
    a_Gyro->Zero();
}

void Autonomous::DecidePath() {

    if (a_Joystick->GetRawButton(11)) {

        autoPathMaster = 0;

    } else if (a_Joystick->GetRawButton(10)) {

        autoPathMaster = 1;

    }

    else if (a_Joystick->GetRawButton(12)) {

        autoPathMaster = 2;
    }
}



void Autonomous::DecidePath(int intent) {

    autoPathMaster = intent;
}

int Autonomous::GetCurrentPath() {

    return autoPathMaster;
}

void Autonomous::StartPathMaster() {

    switch (autoPathMaster) {

        case -1:
            // Error!
            frc::SmartDashboard::PutBoolean("Auto Failed", false);

            break;

        case 0:
            frc::SmartDashboard::PutBoolean("0-ball Taxi started", true);
            AutonomousStart0();

            break;

        case 1:
            frc::SmartDashboard::PutBoolean("1-ball Taxi started", true);
            AutonomousStart1();

            break;

        case 2:
            frc::SmartDashboard::PutBoolean("2-ball Taxi started", true);
            AutonomousStart2();

            break;
    }
}

void Autonomous::StartPathMaster(int path) {

    switch (path) {

        case -1:
            // Error!
            frc::SmartDashboard::PutBoolean("Auto Failed", false);

            break;

        case 0:
            frc::SmartDashboard::PutBoolean("0-ball Taxi Started", true);
            AutonomousStart0();

            break;

        case 1:
            frc::SmartDashboard::PutBoolean("1-ball Taxi Started", true);
            AutonomousStart1();

            break;

        case 2:
            frc::SmartDashboard::PutBoolean("2-ball Taxi Started", true);
            AutonomousStart2();

            break;
    }
}

void Autonomous::PeriodicPathMaster() {
    switch (autoPathMaster) {
        case -1:
            // Error!
            break;
        case 0:
            AutonomousPeriodic0();

            break;

        case 1:
            AutonomousPeriodic1();

            break;

        case 2:
            AutonomousPeriodic2();

            break;
    }
}

void Autonomous::PeriodicPathMaster(int path) {
    switch (path) {
        case -1:
            // Error!
            break;
        case 0:
            AutonomousPeriodic0();

            break;

        case 1:
            AutonomousPeriodic1();

            break;

        case 2:
            AutonomousPeriodic2();

            break;
    }
}


// ----------------------------------AUTONOMOUS ROUTINES---------------------------------------- //

void Autonomous::AutonomousStart0() {

    a_AutoState0 = kDriveAway0;
    a_Gyro->Zero();
}

void Autonomous::AutonomousPeriodic0() {

    AutoState0 nextState = a_AutoState0;

    switch (a_AutoState0) {

        case kAutoIdle0:
            IDontLikeExercise();

            break;

        case kDriveAway0:
            if (DriveDist(36, 0)) {
                nextState = kAutoIdle0;
            }
            break;
    }
    a_AutoState0 = nextState;
}

void Autonomous::AutonomousStart1() {

    a_AutoState1 = kShoot1;
    a_Gyro->Zero();
}


void Autonomous::AutonomousPeriodic1() {

    AutoState1 nextState = a_AutoState1;

    switch (a_AutoState1) {

        case kAutoIdle1:
            IDontLikeExercise();
            break;

        case kShoot1:
            if (IndexAndShoot(SHOOT_FROM_WALL)) {
                nextState = kStartTimer1_1;
            }
            break;

        case kStartTimer1_1:
            StartTimer();
            nextState = kWait1_1;
            break;

        case kWait1_1:
            if (WaitForTime(1)) {
                nextState = kDoneShooting1;
            }

            break;

        case kDoneShooting1:
            IDontLikeExercise();
            nextState = kStartTimer2_1;
            break;

        case kStartTimer2_1:
            StartTimer();
            nextState = kTaxi1;
            break;

        case kTaxi1:
            if (WaitForTime(4)) {
                nextState = kAutoIdle1;
            } else {
                DriveDirection(0.25, 180);
            }
            break;
    }
    a_AutoState1 = nextState;
}
/*
void Autonomous::AutonomousStart2() {
    a_AutoState2 = kCollectDown2;
    a_Gyro->Zero();
}

void Autonomous::AutonomousPeriodic2() {

    AutoState2 nextState = a_AutoState2;

    switch (a_AutoState2) {
        case kAutoIdle2:
            IDontLikeExercise();

            break;

        case kCollectDown2:
            ToggleCollector();
            nextState = kDriveBackThroughBall2;
            break;

        case kDriveBackThroughBall2:
            if (DriveWhileCollecting(36, 0)) {
                nextState = kLoad2;
            }
            break;

        case kLoad2:
            ToggleCollector();
            nextState = kTurn2;
            break;

        case kTurn2:
            if (TurnToAngle(-159)) {
                nextState = kSpool2;
            }
            break;

        case kSpool2:
            SpoolShooter(SHOOT_FROM_WALL);
            nextState = kDriveToWall2;
            break;

        case kDriveToWall2:
            if (IHaveAProposal(0.3, 120, 0)) {
                nextState = kShoot2;
            }
            break;

        case kShoot2:
            if (IndexAndShoot(SHOOT_FROM_WALL)) {
                nextState = kWait1_2;
            }
            break;

        case kWait1_2:
            if (WaitForTime(1)) {
                nextState = kSecondShoot2;
            }
            break;

        case kSecondShoot2:
            if (IndexAndShoot(SHOOT_FROM_WALL)) {
                nextState = kWait2_2;
            }
            break;

        case kWait2_2:
            if (WaitForTime(1)) {
                nextState = kAutoIdle2;
            }
            break;
    }
    a_AutoState2 = nextState;
}
*/
void Autonomous::IDontLikeExercise() {

    a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->getAngle(), true);
    a_Collector->setCollectorMotorSpeed(0);
    a_Collector->setIndexerMotorSpeed(0);
}

void Autonomous::StartTimer() {
    a_Timer->Start();
}

bool Autonomous::WaitForTime(double time) {

    //-----Another case must be written to use StartTimer, though this will handle stopping and resetting-----//
    if (a_Timer->Get().value() < time) {
        return false;
    } else {
        a_Timer->Stop();
        a_Timer->Reset();
        return true;
    }
}


void Autonomous::SpoolShooter(float speed) {
    a_BallShooter->setSpeed(speed);
}

void Autonomous::ToggleCollector() {
    a_Collector->toggleSolenoid();
}

bool Autonomous::DriveDist(double dist, double angle) { // true is done, false is not done

    if ((double) fabs(a_SwerveDrive->getAvgDistance()) < dist) {
        a_SwerveDrive->driveDistance(dist, angle);
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;


    } else {
        a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->getAngle(), true);
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;
    }
}

bool Autonomous::IndexAndShoot(float speed) { // returns true if the shooter is running correctly and the indexer has switched on
    a_BallShooter->setSpeed(speed);

    if(a_BallShooter->getSpeed() >= speed - 200) {
        a_Collector->setIndexerMotorSpeed(COLLECTOR_MOTOR_PERCENT_OUTPUT);
        return true;
    } else if (a_BallShooter->getSpeed() < speed - 200) {
        return false;
    }
}

bool Autonomous::TurnToAngle(float angle) {

    if (fabs(a_Gyro->getAngle() - angle) >= 1) {
        a_SwerveDrive->turnToAngle(a_Gyro->getAngle(), angle);
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;


    } else {
        a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->getAngle(), true);
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;
    }
}
/*
bool Autonomous::BallShot(float speed) { // looks for a dip in RPM value to detect a shot being made
    // going to reimplement using beambreak soon
    if (a_BeamBreak->beamBroken()) {
        a_Collector->setIndexerMotorSpeed(0);
        return true;
    } else if (a_BallShooter->getSpeed() >= speed) {
        return false;
    }
}
*/
bool Autonomous::DriveDirection(float speed, float angle) {
    a_SwerveDrive->driveDirection(speed, angle);
}

bool Autonomous::IHaveAProposal(float speed, float dir, float dist) { // true is done, false is not done

    if (fabs(a_SwerveDrive->getAvgDistance()) < (dist + drivestart)) {

        if (a_SwerveDrive->getAvgDistance() > (0.80 * (dist + drivestart))) {
            a_SwerveDrive->goToTheDon(speed / 2, dir, dist, a_Gyro->getAngle());
            a_BallShooter->setSpeed(AUTO_SHOOT_VELOCITY);
        } else {
            a_SwerveDrive->goToTheDon(speed, dir, dist, a_Gyro->getAngle());
            a_BallShooter->setSpeed(0);
        }
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;

    } else {
        a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->getAngle(), true);
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;
    }
}

bool Autonomous::DriveWhileCollecting(double dist, double angle) {
    a_Collector->setCollectorMotorSpeed(COLLECTOR_MOTOR_PERCENT_OUTPUT);
    if (DriveDist(dist, angle)) {
        a_Collector->setCollectorMotorSpeed(COLLECTOR_MOTOR_PERCENT_OUTPUT);
        return true;
    } else {
        return false;
    }
}