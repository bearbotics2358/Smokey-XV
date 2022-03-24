#include "Autonomous.h"
#include "buttons.h"
#include "misc.h"
#include <math.h>


Autonomous::Autonomous(JrimmyGyro *Gyro, frc::Joystick *Xbox_Controller, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector):
a_Gyro(Gyro),
a_SwerveDrive(SwerveDrive),
a_Xbox(Xbox_Controller),
a_BallShooter(BallShooter),
a_Collector(Collector),
a_AutoState0(kAutoIdle0),
a_AutoState1(kAutoIdle1),
a_AutoState2(kAutoIdle2) {
    autoPathMaster = k2Ball;
}

void Autonomous::Init() {
    a_Gyro->Zero();
}

void Autonomous::DecidePath() {
    if (a_Xbox->GetRawAxis(OperatorJoystick::LeftTrigger) > 0.5) {
        if (a_Xbox->GetRawButtonPressed(OperatorButton::Y)) {
            if (autoPathMaster == k0Ball) {
                autoPathMaster = k5BallVision;
            } else {
                autoPathMaster = (AutoType) (autoPathMaster - 1);
            }
        }
        if (a_Xbox->GetRawButtonPressed(OperatorButton::A)) {
            if (autoPathMaster == k5BallVision) {
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
            return "0 ball auto chosen";
        case kLeft1Ball:
            return "left 1 ball auto chosen";
        case kMiddle1Ball:
            return "middle 1 ball auto chosen";
        case kRight1Ball:
            return "right 1 ball auto chosen";
        case k2Ball:
            return "2 ball auto chosen";
        case k5Ball:
            return "5 ball auto chosen";
        case k5BallVision:
            return "5 ball vision auto chosen";
        default:
            return "no autonous selected, this shouldn't happen";
    }
}

void Autonomous::StartPathMaster() {
    switch (autoPathMaster) {
        case k0Ball:
            Start0Ball();
            break;
        case kLeft1Ball:
            StartLeft1Ball();
            break;
        case kMiddle1Ball:
            StartMiddle1Ball();
            break;
        case kRight1Ball:
            StartRight1Ball();
            break;
        case k2Ball:
            Start2Ball();
            break;
        case k5Ball:
        case k5BallVision:
            Start5Ball();
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
        case k5Ball:
            Periodic5Ball();
            break;
        case k5BallVision:
            Periodic5BallVision();
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
            if (DriveDirection(1.32, 133, 0.25, true)) {
                CollectorUp();
                SpoolShooter(SHOOTER_SPEED);
                nextState = kTurn2;
            }
            break;

        case kTurn2:
            if (TurnToAngle(-21)) {
                nextState = kDriveToWall2;
                StartTimer();
            }
            break;

        case kDriveToWall2:
            // we might be stuck on the wall, so move to the next state after some time
            if (DriveDirection(2.23, -37, 0.25, true) || WaitForTime(5)) {
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

void Autonomous::Start5Ball() {
    a_AutoState5 = A5::SpoolShooter;
    a_Gyro->Zero(69);
    a_SwerveDrive->setPosition(AUTO5_START_POS);
}

void Autonomous::Periodic5Ball() {
    A5 nextState = a_AutoState5;

    switch (nextState) {
        case A5::Idle:
            IDontLikeExercise();
            break;
        case A5::SpoolShooter:
            SpoolShooter(SHOOTER_SPEED);
            StartTimer();
            nextState = A5::WaitShooter;
            break;
        case A5::WaitShooter:
            if (WaitForTime(0.5) || a_BallShooter->getSpeed() > SHOOTER_TOLERANCE * SHOOTER_SPEED) {
                StartTimer();
                a_Collector->setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_OUTPUT);
                nextState = A5::Shoot1;
            }
            break;
        case A5::Shoot1:
            if (WaitForTime(0.5)) {
                a_Collector->setIndexerMotorSpeed(0);
                StartTimer();
                nextState = A5::Pickup2;
            }
            break;
        case A5::Pickup2:
            // to avoid deploying collector against the wall
            if (WaitForTime(0.1)) {
                CollectorDown();
            }
            if (a_SwerveDrive->goToPosition(Vec2(7.94, 7.57), 260, AUTO5_SPEED)) {
                nextState = A5::Pickup3;
            }
            break;
        case A5::Pickup3:
            if (a_SwerveDrive->goToPosition(Vec2(6.36, 5.05), 155, AUTO5_SPEED)) {
                CollectorUp();
                nextState = A5::GoToShoot23;
            }
            break;
        case A5::GoToShoot23:
            if (a_SwerveDrive->goToPosition(AUTO5_START_POS, 69, AUTO5_SPEED)) {
                a_SwerveDrive->coastStop();
                StartTimer();
                a_Collector->setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_OUTPUT);
                nextState = A5::Shoot23;
            }
            break;
        case A5::Shoot23:
            if (WaitForTime(1.5)) {
                a_Collector->setIndexerMotorSpeed(0);
                StartTimer();
                nextState = A5::Pickup4;
            }
            break;
        case A5::Pickup4:
            // to avoid deploying collector against the wall
            if (WaitForTime(0.1)) {
                CollectorDown();
            }
            if (a_SwerveDrive->goToPosition(Vec2(6.812, 1.352), 225, AUTO5_SPEED)) {
                a_SwerveDrive->coastStop();
                StartTimer();
                nextState = A5::WaitPickup5;
            }
            break;
        case A5::WaitPickup5:
            // wait for the human player to put the ball out
            if (WaitForTime(2)) {
                CollectorUp();
                nextState = A5::GoToShoot45;
            }
            break;
        case A5::GoToShoot45:
            if (a_SwerveDrive->goToPosition(AUTO5_START_POS, 69, AUTO5_SPEED)) {
                a_SwerveDrive->coastStop();
                StartTimer();
                a_Collector->setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_OUTPUT);
                nextState = A5::Shoot45;
            }
            break;
        case A5::Shoot45:
            if (WaitForTime(1.5)) {
                nextState = A5::Idle;
            }
            break;
    }

    a_AutoState5 = nextState;
}

void Autonomous::Periodic5BallVision() {
    A5V nextState = a_AutoState5Vision;

    switch (nextState) {
        case A5V::Idle:
            IDontLikeExercise();
            break;
        case A5V::SpoolShooter:
            SpoolShooter(SHOOTER_SPEED);
            StartTimer();
            nextState = A5V::WaitShooter;
            break;
        case A5V::WaitShooter:
            if (WaitForTime(0.5) || a_BallShooter->getSpeed() > SHOOTER_TOLERANCE * SHOOTER_SPEED) {
                StartTimer();
                a_Collector->setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_OUTPUT);
                nextState = A5V::Shoot1;
            }
            break;
        case A5V::Shoot1:
            if (WaitForTime(0.5)) {
                a_Collector->setIndexerMotorSpeed(0);
                StartTimer();
                nextState = A5V::Pickup2;
            }
            break;
        case A5V::Pickup2:
            // to avoid deploying collector against the wall
            if (WaitForTime(0.1)) {
                CollectorDown();
            }
            if (a_SwerveDrive->goToPosition(Vec2(7.94, 7.57), 260, AUTO5_SPEED)) {
                nextState = A5V::Pickup3;
            }
            break;
        case A5V::Pickup3:
            if (a_SwerveDrive->goToPosition(Vec2(6.36, 5.05), 155, AUTO5_SPEED)) {
                CollectorUp();
                nextState = A5V::GoToShoot23;
            }
            break;
        case A5V::GoToShoot23:
            if (a_SwerveDrive->goToPosition(AUTO5_START_POS, 69, AUTO5_SPEED)) {
                StartTimer();
                a_Collector->setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_OUTPUT);
                nextState = A5V::Shoot23;
            }
            break;
        case A5V::Shoot23:
            if (WaitForTime(1.5)) {
                a_Collector->setIndexerMotorSpeed(0);
                StartTimer();
                nextState = A5V::Pickup4;
            }
            break;
        case A5V::Pickup4:
            // to avoid deploying collector against the wall
            if (WaitForTime(0.1)) {
                CollectorDown();
            }
            if (a_SwerveDrive->goToPosition(Vec2(6.812, 1.352), 225, AUTO5_SPEED)) {
                StartTimer();
                nextState = A5V::WaitPickup5;
            }
            break;
        case A5V::WaitPickup5:
            // wait for the human player to put the ball out
            if (WaitForTime(2)) {
                CollectorUp();
                nextState = A5V::GoToShoot45;
            }
            break;
        case A5V::GoToShoot45:
            if (a_SwerveDrive->goToPosition(AUTO5_START_POS, 69, AUTO5_SPEED)) {
                StartTimer();
                a_Collector->setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_OUTPUT);
                nextState = A5V::Shoot45;
            }
            break;
        case A5V::Shoot45:
            if (WaitForTime(1.5)) {
                nextState = A5V::Idle;
            }
            break;
    }

    a_AutoState5Vision = nextState;
}

void Autonomous::IDontLikeExercise() {
    a_SwerveDrive->coastStop();
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
    a_Collector->setCollectorMotorSpeed(COLLECTOR_MOTOR_PERCENT_OUTPUT);
}

void Autonomous::CollectorUp() {
    a_Collector->setSolenoid(false);
    a_Collector->setCollectorMotorSpeed(0);
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
        a_SwerveDrive->turnToAngle(angle);
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;


    } else {
        a_SwerveDrive->coastStop();
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;
    }
}

bool Autonomous::DriveDirection(double dist, double angle, double speed, bool fieldOriented) { // true is done, false is not done

    if (fabs(a_SwerveDrive->getAvgDistance()) < (dist + drivestart)) {

        if (a_SwerveDrive->getAvgDistance() > (0.80 * (dist + drivestart))) {
            a_SwerveDrive->goToTheDon(speed / 2, angle, dist, fieldOriented);

        } else {
            a_SwerveDrive->goToTheDon(speed, angle, dist, fieldOriented);
        }
        frc::SmartDashboard::PutNumber("Encoder average?????", a_SwerveDrive->getAvgDistance());
        return false;

    } else {
        a_SwerveDrive->coastStop();
        a_SwerveDrive->unsetHoldAngle();
        frc::SmartDashboard::PutNumber("We done????? ", a_SwerveDrive->getAvgDistance());
        return true;
    }
}