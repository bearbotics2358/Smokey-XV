#include "Autonomous.h"
#include "buttons.h"
#include "misc.h"
#include <math.h>


Autonomous::Autonomous(JrimmyGyro *Gyro, frc::Joystick *Joystick, frc::Joystick *Xbox_Controller, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector):
a_Gyro(Gyro),
a_Joystick(Joystick),
a_SwerveDrive(SwerveDrive),
a_Xbox(Xbox_Controller),
a_BallShooter(BallShooter),
a_Collector(Collector),
a_AutoState0(kAutoIdle0),
a_AutoState1(kAutoIdle1),
a_AutoState2(kAutoIdle2) {
    autoPathMaster = k2Ball;
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
    if (a_Xbox->GetRawAxis(OperatorJoystick::LeftTrigger) > 0.5) {
        if (a_Xbox->GetRawButtonPressed(OperatorButton::Y)) {
            if (autoPathMaster == k0Ball) {
                autoPathMaster = k2Ball;
            } else {
                autoPathMaster = (AutoType) (autoPathMaster - 1);
            }
        }
        if (a_Xbox->GetRawButtonPressed(OperatorButton::A)) {
            if (autoPathMaster == k2Ball) {
                autoPathMaster = k0Ball;
            } else {
                autoPathMaster = (AutoType) (autoPathMaster + 1);
            }
        }
    }
}

const char *Autonomous::GetCurrentPath() {
    switch (autoPathMaster) {
        case k0Ball:
            return "0-ball taxi chosen";
        case kLeft1Ball:
            return "left 1-ball taxi chosen";
        case kMiddle1Ball:
            return "middle 1-ball taxi chosen";
        case kRight1Ball:
            return "right 1-ball taxi chosen";
        case k2Ball:
            return "2-ball taxi chosen";
        default:
            return "no autonous selected, this shouldn't happen";
    }
}

void Autonomous::StartPathMaster() {
    switch (autoPathMaster) {
        case k0Ball:
            frc::SmartDashboard::PutBoolean("0-ball Taxi started", true);
            Start0Ball();
            break;
        case kLeft1Ball:
            frc::SmartDashboard::PutBoolean("left 1-ball Taxi started", true);
            StartLeft1Ball();
            break;
        case kMiddle1Ball:
            frc::SmartDashboard::PutBoolean("middle 1-ball Taxi started", true);
            StartMiddle1Ball();
            break;
        case kRight1Ball:
            frc::SmartDashboard::PutBoolean("right 1-ball taxi started", true);
            StartRight1Ball();
            break;
        case k2Ball:
            frc::SmartDashboard::PutBoolean("2-ball Taxi started", true);
            Start2Ball();
            break;
    }
}

void Autonomous::PeriodicPathMaster() {
    switch (autoPathMaster) {
        case k0Ball:
            Periodic0Ball();
            break;
        case kLeft1Ball:
        case kMiddle1Ball:
        case kRight1Ball:
            Periodic1Ball();
            break;
        case k2Ball:
            Periodic2Ball();
            break;
    }
}

// ----------------------------------AUTONOMOUS ROUTINES---------------------------------------- //

void Autonomous::Start0Ball() {
    a_AutoState0 = kDriveAway0;
    a_Gyro->Zero();
}

void Autonomous::Periodic0Ball() {

    AutoState0 nextState = a_AutoState0;

    switch (a_AutoState0) {

        case kAutoIdle0:
            IDontLikeExercise();

            break;

        case kDriveAway0:
            if (DriveDirection(1.0, 0, 0.25, false)) {
                nextState = kAutoIdle0;
            }
            break;
    }
    a_AutoState0 = nextState;
}

void Autonomous::StartLeft1Ball() {
    a_AutoState1 = kStartShooter1;
    a_Gyro->Zero(-21);
}

void Autonomous::StartMiddle1Ball() {
    a_AutoState1 = kStartShooter1;
    a_Gyro->Zero();
}

void Autonomous::StartRight1Ball() {
    a_AutoState1 = kStartShooter1;
    a_Gyro->Zero(69);
}

void Autonomous::Periodic1Ball() {

    AutoState1 nextState = a_AutoState1;

    switch (a_AutoState1) {

        case kAutoIdle1:
            IDontLikeExercise();
            break;

        case kStartShooter1:
            SpoolShooter(SHOOTER_SPEED);
            StartTimer();
            nextState = kWaitShooter1;
            break;
        
        case kWaitShooter1:
            if (WaitForTime(2)) {
                nextState = kShoot1;
            }
            break;

        case kShoot1:
            a_Collector->setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_OUTPUT);
            nextState = kStartTimer1;
            break;

        case kStartTimer1:
            StartTimer();
            nextState = kWait1;
            break;

        case kWait1:
            if (WaitForTime(1)) {
                nextState = kDoneShooting1;
            }
            break;

        case kDoneShooting1:
            IDontLikeExercise();
            nextState = kTaxi1;
            break;

        case kTaxi1:
            if (DriveDirection(2.4, 180, 0.25, false)) {
                nextState = kAutoIdle1;
            }
            break;
    }
    a_AutoState1 = nextState;
}

void Autonomous::Start2Ball() {
    a_AutoState2 = kDriveBackThroughBall2;
    a_Gyro->Zero(133);
}

void Autonomous::Periodic2Ball() {

    AutoState2 nextState = a_AutoState2;

    switch (a_AutoState2) {
        case kAutoIdle2:
            IDontLikeExercise();

            break;

        case kDriveBackThroughBall2:
            CollectorDown();
            CollectorOn();
            if (DriveDirection(1.32, 133, 0.25, true)) {
                CollectorUp();
                CollectorOff();
                SpoolShooter(SHOOTER_SPEED);
                nextState = kTurn2;
            }
            break;

        case kTurn2:
            if (TurnToAngle(-21)) {
                nextState = kDriveToWall2;
            }
            break;

        case kDriveToWall2:
            if (DriveDirection(2.23, -37, 0.25, true)) {
                nextState = kShoot2;
            }
            break;

        case kShoot2:
            a_Collector->setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_OUTPUT);
            StartTimer();
            nextState = kWait2;
            break;

        case kWait2:
            if (WaitForTime(3)) {
                nextState = kAutoIdle2;
                a_Collector->setIndexerMotorSpeed(0);
            }
            break;
    }
    a_AutoState2 = nextState;
}

void Autonomous::IDontLikeExercise() {

    a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->getAngle(), true);
    a_Collector->setCollectorMotorSpeed(0);
    a_Collector->setIndexerMotorSpeed(0);
}

void Autonomous::StartTimer() {
    waitTimeStart = misc::getSeconds();
}

bool Autonomous::WaitForTime(double time) {
    return misc::getSeconds() > waitTimeStart + time;
}


void Autonomous::SpoolShooter(float speed) {
    a_BallShooter->setSpeed(speed);
}

void Autonomous::CollectorDown() {
    a_Collector->setSolenoid(true);
}

void Autonomous::CollectorUp() {
    a_Collector->setSolenoid(false);
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

    if (a_BallShooter->getSpeed() >= speed * SHOOTER_TOLERANCE) {
        a_Collector->setIndexerMotorSpeed(COLLECTOR_MOTOR_PERCENT_OUTPUT);
        return true;
    } else {
        return false;
    }
}

bool Autonomous::TurnToAngle(float angle) { // rotates bot in place to specific angle

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

bool Autonomous::IHaveAProposal(float speed, float dir, float dist) { // true is done, false is not done

    if (fabs(a_SwerveDrive->getAvgDistance()) < (dist + drivestart)) {

        if (a_SwerveDrive->getAvgDistance() > (0.80 * (dist + drivestart))) {
            a_SwerveDrive->goToTheDon(speed / 2, dir, dist, a_Gyro->getAngle());
            a_BallShooter->setSpeed(SHOOTER_SPEED);
        } else {
            a_SwerveDrive->goToTheDon(speed, dir, dist, a_Gyro->getAngle());
            a_BallShooter->stop();
        }
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;

    } else {
        a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->getAngle(), true);
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;
    }
}

void Autonomous::CollectorOn() {
    a_Collector->setCollectorMotorSpeed(COLLECTOR_MOTOR_PERCENT_OUTPUT);
}
void Autonomous::CollectorOff() {
    a_Collector->setCollectorMotorSpeed(0);
}

bool Autonomous::DriveDirection(double dist, double angle, double speed, bool fieldOriented) { // true is done, false is not done

    if (fabs(a_SwerveDrive->getAvgDistance()) < (dist + drivestart)) {

        if (a_SwerveDrive->getAvgDistance() > (0.80 * (dist + drivestart))) {
            a_SwerveDrive->goToTheDon(speed / 2, angle, dist, a_Gyro->getAngle(), fieldOriented);

        } else {
            a_SwerveDrive->goToTheDon(speed, angle, dist, a_Gyro->getAngle(), fieldOriented);
        }
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;

    } else {
        a_SwerveDrive->swerveUpdate(0, 0, 0, a_Gyro->getAngle(), true);
        a_SwerveDrive->unsetHoldAngle();
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;
    }
}