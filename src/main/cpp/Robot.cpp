
#include "Robot.h"
#include "Autonomous.h"
#include "Climber.h"
#include "Prefs.h"
#include "buttons.h"
#include "misc.h"
#include <JrimmyGyro.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <iostream>
#include <stdio.h>

/*~~ hi :) ~~ */
Robot::Robot():
a_Gyro(frc::I2C::kMXP), // 1
a_FLModule(FL_DRIVE_ID, FL_STEER_ID, AbsoluteEncoder(FL_SWERVE_ABS_ENC_PORT, FL_SWERVE_ABS_ENC_MIN_VOLTS, FL_SWERVE_ABS_ENC_MAX_VOLTS, FL_SWERVE_ABS_ENC_OFFSET / 360)),
a_FRModule(FR_DRIVE_ID, FR_STEER_ID, AbsoluteEncoder(FR_SWERVE_ABS_ENC_PORT, FR_SWERVE_ABS_ENC_MIN_VOLTS, FR_SWERVE_ABS_ENC_MAX_VOLTS, FR_SWERVE_ABS_ENC_OFFSET / 360)),
a_BLModule(BL_DRIVE_ID, BL_STEER_ID, AbsoluteEncoder(BL_SWERVE_ABS_ENC_PORT, BL_SWERVE_ABS_ENC_MIN_VOLTS, BL_SWERVE_ABS_ENC_MAX_VOLTS, BL_SWERVE_ABS_ENC_OFFSET / 360)),
a_BRModule(BR_DRIVE_ID, BR_STEER_ID, AbsoluteEncoder(BR_SWERVE_ABS_ENC_PORT, BR_SWERVE_ABS_ENC_MIN_VOLTS, BR_SWERVE_ABS_ENC_MAX_VOLTS, BR_SWERVE_ABS_ENC_OFFSET / 360)),
a_Autonomous(&a_Gyro, &a_Timer, &joystickOne, &a_SwerveDrive, &a_Shooter, &a_Collector, &a_BeamBreak),
joystickOne(JOYSTICK_PORT),
a_XboxController(XBOX_CONTROLLER),
a_SwerveDrive(a_FLModule, a_FRModule, a_BLModule, a_BRModule),
a_Shooter(LEFT_SHOOTER_ID, RIGHT_SHOOTER_ID),
a_Collector(COLLECTOR_MOTOR_ID, INDEXER_MOTOR_ID, SOLENOID_ID, COLLECTOR_PUSH_SOLENOID_MODULE, COLLECTOR_PULL_SOLENOID_MODULE),
a_Climber(CLIMBER_MOTOR_ID, CLIMBER_PUSH_SOLENOID_MODULE, CLIMBER_PULL_SOLENOID_MODULE),
a_CompressorController(),
a_BeamBreak(0), // I NEEDED A PORT, THIS IS PROBABLY WRONG, PLEASE FIX IT LATER
// handler("169.254.179.144", "1185", "data"),
// handler("raspberrypi.local", 1883, "PI/CV/SHOOT/DATA"),
// a_canHandler(CanHandler::layout2022()),
a_shooterVision(SHOOTER_CAMERA_NAME, TargetTracker::Mode::target(0)),
a_ballTracker(SHOOTER_CAMERA_NAME, TargetTracker::Mode::ball(0)) {
    /*if (!handler.ready()) {
        // do something if handler failed to connect
    }*/

    a_FLModule.setDrivePID(0.001, 0, 0);
    a_FLModule.setSteerPID(2.0, 0, 0.01);

    a_FRModule.setDrivePID(0.001, 0, 0);
    a_FRModule.setSteerPID(2.0, 0, 0.01);

    a_BLModule.setDrivePID(0.001, 0, 0);
    a_BLModule.setSteerPID(2.0, 0, 0.01);

    a_BRModule.setDrivePID(0.001, 0, 0);
    a_BRModule.setSteerPID(2.0, 0, 0.01);
}

void Robot::RobotInit() {
    frc::SmartDashboard::init();
    a_Gyro.Init();
    a_Gyro.Zero();
}

void Robot::RobotPeriodic() {
    a_Gyro.Update();
    // handler.update();

    frc::SmartDashboard::PutNumber("Distance Driven: ", a_SwerveDrive.getAvgDistance());
    frc::SmartDashboard::PutNumber("Gyro Angle: ", a_Gyro.getAngle());
    frc::SmartDashboard::PutBoolean("Collector Solenoid Toggle: ", a_Collector.getValue());

    frc::SmartDashboard::PutNumber("Tank Pressure", a_CompressorController.getTankPressure());

    // a_canHandler.update();
    frc::SmartDashboard::PutNumber("Desired Shooter RPM", shooterDesiredSpeed);
    frc::SmartDashboard::PutNumber("Current Shooter RPM", a_Shooter.getSpeed());

    frc::SmartDashboard::PutNumber("Climber Arm Height (mm)", a_Climber.getHeight());
    frc::SmartDashboard::PutNumber("Climber Arm Speed (mm/s)", a_Climber.getSpeed());
    frc::SmartDashboard::PutNumber("Climber Arm Ticks Raised", a_Climber.getTicks());

    frc::SmartDashboard::PutNumber("FL rel encoder", a_FLModule.getAngle());
    frc::SmartDashboard::PutNumber("FR rel encoder", a_FRModule.getAngle());
    frc::SmartDashboard::PutNumber("BL rel encoder", a_BLModule.getAngle());
    frc::SmartDashboard::PutNumber("BR rel encoder", a_BRModule.getAngle());

    frc::SmartDashboard::PutNumber("FL abs encoder", 360 - a_FLModule.getAbsAngleDegrees());
    frc::SmartDashboard::PutNumber("FR abs encoder", 360 - a_FRModule.getAbsAngleDegrees());
    frc::SmartDashboard::PutNumber("BL abs encoder", 360 - a_BLModule.getAbsAngleDegrees());
    frc::SmartDashboard::PutNumber("BR abs encoder", 360 - a_BRModule.getAbsAngleDegrees());

    frc::SmartDashboard::PutNumber("FL raw rel encoder", a_FLModule.getRelativeAngle());
    frc::SmartDashboard::PutNumber("FR raw rel encoder", a_FRModule.getRelativeAngle());
    frc::SmartDashboard::PutNumber("BL raw rel encoder", a_BLModule.getRelativeAngle());
    frc::SmartDashboard::PutNumber("BR raw rel encoder", a_BRModule.getRelativeAngle());
    //
    frc::SmartDashboard::PutNumber("FL volts", a_FLModule.getAbsEncoderVolts());
    frc::SmartDashboard::PutNumber("FR volts", a_FRModule.getAbsEncoderVolts());
    frc::SmartDashboard::PutNumber("BL volts", a_BLModule.getAbsEncoderVolts());
    frc::SmartDashboard::PutNumber("BR volts", a_BRModule.getAbsEncoderVolts());

    frc::SmartDashboard::PutNumber("FL offset", a_FLModule.encZeroPoint);
    frc::SmartDashboard::PutNumber("FR offset", a_FRModule.encZeroPoint);
    frc::SmartDashboard::PutNumber("BL offset", a_BLModule.encZeroPoint);
    frc::SmartDashboard::PutNumber("BR offset", a_BRModule.encZeroPoint);
}

void Robot::DisabledInit() {
    a_doEnabledInit = true;
    a_SwerveDrive.resetDrive();
    shooterDesiredSpeed = 0.0;
}

void Robot::DisabledPeriodic() {
    a_Autonomous.DecidePath();
    frc::SmartDashboard::PutNumber("Selected Autonomous", a_Autonomous.GetCurrentPath());
}

void Robot::EnabledInit() {
    a_Collector.resetSolenoid();
    a_Climber.resetClimber();
}

void Robot::EnabledPeriodic() {
    a_shooterVision.update();
    a_ballTracker.update();
    a_CompressorController.update();
}

void Robot::AutonomousInit() {
    if (a_doEnabledInit) {
        EnabledInit();
        a_doEnabledInit = false;
    }

    a_Autonomous.Init();
    a_Autonomous.StartPathMaster();
}

void Robot::AutonomousPeriodic() {
    EnabledPeriodic();

    a_Autonomous.PeriodicPathMaster();
}

void Robot::TeleopInit() {
    if (a_doEnabledInit) {
        EnabledInit();
        a_doEnabledInit = false;
    }
}

// main loop
void Robot::TeleopPeriodic() {
    EnabledPeriodic();

    /* =-=-=-=-=-=-=-=-=-=-= Climber Controls =-=-=-=-=-=-=-=-=-=-= */

    if (a_XboxController.GetRawButton(OperatorButton::LeftBumper)) {
        a_Climber.setArmSpeed(CLIMBER_MOTOR_PERCENT_OUTPUT); // 100% power
    } else if (a_XboxController.GetRawButton(OperatorButton::RightBumper)) {
        a_Climber.setArmSpeed(-CLIMBER_MOTOR_PERCENT_OUTPUT);
    } else {
        a_Climber.setArmSpeed(0);
    }
    if (a_XboxController.GetRawButtonPressed(OperatorButton::Back)) {
        a_Climber.toggleSolenoid();
    }

    /* =-=-=-=-=-=-=-=-=-=-= Shooter Controls =-=-=-=-=-=-=-=-=-=-= */


    /*if (a_XboxController.GetRawButton(OperatorButton::A)) {
        shooterDesiredSpeed += 10.0;
    }
    if (a_XboxController.GetRawButton(OperatorButton::B)) {
        shooterDesiredSpeed -= 10.0;
    }*/

    // a_Shooter.setSpeed(shooterDesiredSpeed);

    if (a_XboxController.GetRawButtonPressed(OperatorButton::A)) {
        a_Shooter.setSpeed(SHOOTER_SPEED);
        // TODO: decrease margin of error when better pid tuned
        if (a_Shooter.getSpeed() >= 0.8 * SHOOTER_SPEED) {
            a_Collector.setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_POWER);
        }
    } else {
        a_Shooter.setSpeed(0);
        a_Collector.setIndexerMotorSpeed(0);
    }

    /*=-=-=-=-=-=-=-=- Testing Collector Controls -=-=-=-=-=-=-=-=*/

    if (a_XboxController.GetRawButtonPressed(OperatorButton::Start)) {
        a_Collector.toggleSolenoid();
    }
    if (a_XboxController.GetRawButton(OperatorButton::X)) {
        a_Collector.setCollectorMotorSpeed(COLLECTOR_MOTOR_PERCENT_OUTPUT);
    } else {
        a_Collector.setCollectorMotorSpeed(0);
    }
    /*if (a_XboxController.GetRawButton(OperatorButton::Y)) {
        a_Collector.setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_POWER);
    } else {
        a_Collector.setIndexerMotorSpeed(0);
    }*/

    /* =-=-=-=-=-=-=-=-=-=-= Swerve Controls =-=-=-=-=-=-=-=-=-=-= */

    float x = -1 * joystickOne.GetRawAxis(DriverJoystick::XAxis);
    float y = -1 * joystickOne.GetRawAxis(DriverJoystick::YAxis);
    float z = -1 * joystickOne.GetRawAxis(DriverJoystick::ZAxis);
    float gyro = a_Gyro.getAngle();

    if (fabs(x) < 0.10) {
        x = 0;
    }
    if (fabs(y) < 0.10) {
        y = 0;
    }
    if (fabs(z) < 0.10) {
        z = 0;
    }

    if (gyro < 0) {
        gyro = fmod(gyro, -360);
        gyro += 360;
    } else {
        gyro = fmod(gyro, 360);
    }

    bool fieldOreo = true; // field oriented? - yes

    frc::SmartDashboard::PutNumber("Chase: ", z);
    bool inDeadzone = (sqrt(x * x + y * y) < JOYSTICK_DEADZONE) && (fabs(z) < JOYSTICK_DEADZONE); // Checks joystick deadzones

    if (joystickOne.GetRawButton(DriverButton::Button5)) {
        a_Gyro.Cal();
        a_Gyro.Zero();
    }

    // recalibrates the relative encoders using the absolute encoders
    if (joystickOne.GetRawButton(DriverButton::Button6)) {
        a_SwerveDrive.resetDrive();
    }

    if (joystickOne.GetRawButton(DriverButton::Button4)) {
        // vision led on
    } else {
        // vision led off
    }

    if (joystickOne.GetRawButton(DriverButton::Button4)) /* && has target (todo once written) */ {
        // track target with vision
    } else if (joystickOne.GetRawButton(DriverButton::Button3)) {
        if (!inDeadzone) {
            if (joystickOne.GetRawButton(DriverButton::Trigger)) {
                a_SwerveDrive.swerveUpdate(x, y, 0.5 * z, gyro, false);
            } else {
                a_SwerveDrive.crabDriveUpdate(x, y, gyro);
            }
        } else {
            a_SwerveDrive.swerveUpdate(0, 0, 0, gyro, false);
        }
    } else {
        if (!inDeadzone) {
            if (joystickOne.GetRawButton(DriverButton::Trigger)) {
                a_SwerveDrive.swerveUpdate(x, y, 0.5 * z, gyro, fieldOreo);
            } else {
                a_SwerveDrive.crabDriveUpdate(x, y, gyro);
            }
        } else {
            a_SwerveDrive.swerveUpdate(0, 0, 0, gyro, fieldOreo);
        }
    }
}

void Robot::TestInit() {
    if (a_doEnabledInit) {
        EnabledInit();
        a_doEnabledInit = false;
    }
}


void Robot::TestPeriodic() {
    EnabledPeriodic();

    float x = -1 * joystickOne.GetRawAxis(DriverJoystick::XAxis);
    float y = -1 * joystickOne.GetRawAxis(DriverJoystick::YAxis);
    float z = -1 * joystickOne.GetRawAxis(DriverJoystick::ZAxis);
    float gyro = a_Gyro.getAngle();

    if (fabs(x) < 0.10) {
        x = 0;
    }
    if (fabs(y) < 0.10) {
        y = 0;
    }
    if (fabs(z) < 0.10) {
        z = 0;
    }

    if (gyro < 0) {
        gyro = fmod(gyro, -360);
        gyro += 360;
    } else {
        gyro = fmod(gyro, 360);
    }

    bool fieldOreo = true; // field oriented? - yes

    frc::SmartDashboard::PutNumber("Chase: ", z);
    bool inDeadzone = (sqrt(x * x + y * y) < JOYSTICK_DEADZONE) && (fabs(z) < JOYSTICK_DEADZONE); // Checks joystick deadzones


    if (joystickOne.GetRawButton(DriverButton::Button3)) {
        a_SwerveDrive.turnToAngle(gyro, 180.0);
    } else if (!inDeadzone) {
        if (joystickOne.GetRawButton(DriverButton::Trigger)) {
            if (joystickOne.GetRawButton(DriverButton::ThumbButton)) {
                // a_swerveyDrive.makeShiftTurn(a_LimeyLight.calcZAxis());
            } else {
                a_SwerveDrive.swerveUpdate(x, y, 0.5 * z, gyro, fieldOreo);
            }
        } else {
            a_SwerveDrive.crabDriveUpdate(x, y, gyro);
        }
    } else {
        if (joystickOne.GetRawButton(DriverButton::ThumbButton)) {
            // a_swerveyDrive.makeShiftTurn(a_LimeyLight.calcZAxis());
        } else {
            a_SwerveDrive.swerveUpdate(0, 0, 0, gyro, fieldOreo);
        }
    }

    frc::SmartDashboard::PutNumber("Gyro: ", gyro);
}

int main() { return frc::StartRobot<Robot>(); } // Initiate main loop
