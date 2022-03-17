#include "Autonomous.h"
#include "buttons.h"
#include <math.h>


Autonomous::Autonomous(JrimmyGyro *Gyro, frc::Timer *Timer, frc::Joystick *Joystick, frc::Joystick *Xbox_Controller, SwerveDrive *SwerveDrive, BallShooter *BallShooter, Collector *Collector):
a_Gyro(Gyro),
a_Timer(Timer),
a_Joystick(Joystick),
a_SwerveDrive(SwerveDrive),
a_Xbox(Xbox_Controller),
a_BallShooter(BallShooter),
a_Collector(Collector),
a_AutoState0(kAutoIdle0),
a_AutoState1(kAutoIdle1),
a_AutoState1_1(kAutoIdle1_1),
a_AutoState2(kAutoIdle2)

{

    autoPathMaster = 3;
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
        if (a_Xbox->GetPOV() == 0) {
            if (autoPathMaster == 0) {
                autoPathMaster = MAX_AUTO;
            } else {
                autoPathMaster -= 1;
            }
        }
        if (a_Xbox->GetPOV() == 180) {
            if (autoPathMaster == MAX_AUTO) {
                autoPathMaster = 0;
            } else {
                autoPathMaster += 1;
            }
        }
        // allows operator to cycle past normal range bound (which is [0,3]) and loop to other end
        /*
        if(autoPathMaster < 0){
            autoPathMaster = 3;
        }
        if(autoPathMaster > 3){
            autoPathMaster = 0;
        }
        */
    }
}



void Autonomous::DecidePath(int intent) {

    autoPathMaster = intent;
}

const char *Autonomous::GetCurrentPath() {

    if (autoPathMaster == 0) {
        return "0-ball taxi chosen";
    } else if (autoPathMaster == 1) {
        return "left 1-ball taxi chosen";
    } else if (autoPathMaster == 2) {
        return "right 1-ball taxi chosen";
    } else if (autoPathMaster == 3) {
        return "2-ball taxi chosen";
    } else {
        return "no auto chosen";
    }
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
            frc::SmartDashboard::PutBoolean("left 1-ball Taxi started", true);
            AutonomousStart1();

            break;

        case 2:
            frc::SmartDashboard::PutBoolean("right 1-ball taxi started", true);
            AutonomousStart1_1();

        case 3:
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
            frc::SmartDashboard::PutBoolean("left 1-ball Taxi Started", true);
            AutonomousStart1();

            break;

        case 2:
            frc::SmartDashboard::PutBoolean("right 1-ball taxi started", true);
            AutonomousStart1_1();

        case 3:
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
            AutonomousPeriodic1_1();
            break;

        case 3:
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
            if (DriveDirection(1.0, 0, 0.25, false)) {
                nextState = kAutoIdle0;
            }
            break;
    }
    a_AutoState0 = nextState;
}

void Autonomous::AutonomousStart1() {

    a_AutoState1 = kShoot1;
    a_Gyro->Zero(-21);
}

void Autonomous::AutonomousPeriodic1() {

    AutoState1 nextState = a_AutoState1;

    switch (a_AutoState1) {

        case kAutoIdle1:
            IDontLikeExercise();
            break;

        case kShoot1:
            if (IndexAndShoot(SHOOTER_SPEED)) {
                nextState = kStartTimer1;
            }
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
            if (DriveDirection(2.4, 159, 0.25, false)) {
                nextState = kAutoIdle1;
            }
            break;
    }
    a_AutoState1 = nextState;
}

void Autonomous::AutonomousStart1_1() {
    a_AutoState1_1 = kShoot1_1;
    a_Gyro->Zero(69);
}

void Autonomous::AutonomousPeriodic1_1() {

    AutoState1_1 nextState = a_AutoState1_1;

    switch (a_AutoState1) {

        case kAutoIdle1_1:
            IDontLikeExercise();
            break;

        case kShoot1_1:
            if (IndexAndShoot(SHOOTER_SPEED)) {
                nextState = kStartTimer1_1;
            }
            break;

        case kStartTimer1_1:
            StartTimer();
            nextState = kWait1_1;
            break;

        case kWait1_1:
            if (WaitForTime(1)) {
                nextState = kDoneShooting1_1;
            }

            break;

        case kDoneShooting1_1:
            IDontLikeExercise();
            nextState = kTaxi1_1;
            break;

        case kTaxi1_1:
            if (DriveDirection(2.4, 111, 0.25, false)) {
                nextState = kAutoIdle1_1;
            }
            break;
    }
    a_AutoState1_1 = nextState;
}

void Autonomous::AutonomousStart2() {
    a_AutoState2 = kDriveBackThroughBall2;
    a_Gyro->Zero(133);
}


void Autonomous::AutonomousPeriodic2() {

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
                nextState = kTurn2;
            }
            break;

        case kTurn2:
            if (TurnToAngle(-21)) {
                SpoolShooter(SHOOTER_SPEED);
                nextState = kDriveToWall2;
            }
            break;

        case kDriveToWall2:
            if (DriveDirection(2.23, -37, 0.25, true)) {
                nextState = kShoot2;
            }
            break;

        case kShoot2:
            if (IndexAndShoot(SHOOTER_SPEED)) {
                StartTimer();
                nextState = kWait2;
            }
            break;

        case kWait2:
            if (WaitForTime(3)) {
                nextState = kAutoIdle2;
            }
            break;
            /*
                    case kSecondShoot2:
                        if (IndexAndShoot(SHOOTER_SPEED)) {
                            nextState = kWait2_2;
                        }
                        break;

                    case kWait2_2:
                        if (WaitForTime(1)) {
                            nextState = kAutoIdle2;
                        }
                        break;
            */
    }
    a_AutoState2 = nextState;
}

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