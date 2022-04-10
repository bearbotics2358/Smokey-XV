
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
m_gyro(frc::I2C::kMXP),
m_flModule(FL_DRIVE_ID, FL_STEER_ID, AbsoluteEncoder(FL_SWERVE_ABS_ENC_PORT, FL_SWERVE_ABS_ENC_MIN_VOLTS, FL_SWERVE_ABS_ENC_MAX_VOLTS, FL_SWERVE_ABS_ENC_OFFSET / 360)),
m_frModule(FR_DRIVE_ID, FR_STEER_ID, AbsoluteEncoder(FR_SWERVE_ABS_ENC_PORT, FR_SWERVE_ABS_ENC_MIN_VOLTS, FR_SWERVE_ABS_ENC_MAX_VOLTS, FR_SWERVE_ABS_ENC_OFFSET / 360)),
m_blModule(BL_DRIVE_ID, BL_STEER_ID, AbsoluteEncoder(BL_SWERVE_ABS_ENC_PORT, BL_SWERVE_ABS_ENC_MIN_VOLTS, BL_SWERVE_ABS_ENC_MAX_VOLTS, BL_SWERVE_ABS_ENC_OFFSET / 360)),
m_brModule(BR_DRIVE_ID, BR_STEER_ID, AbsoluteEncoder(BR_SWERVE_ABS_ENC_PORT, BR_SWERVE_ABS_ENC_MIN_VOLTS, BR_SWERVE_ABS_ENC_MAX_VOLTS, BR_SWERVE_ABS_ENC_OFFSET / 360)),
m_swerveDrive(m_flModule, m_frModule, m_blModule, m_brModule, m_gyro),
m_autonomous(&m_gyro, &m_xboxController, &m_swerveDrive, &m_shooter, &m_collector),
m_joystick(JOYSTICK_PORT),
m_xboxController(XBOX_CONTROLLER_PORT),
m_shooter(LEFT_SHOOTER_ID, RIGHT_SHOOTER_ID),
m_collector(COLLECTOR_MOTOR_ID, INDEXER_MOTOR_ID, COLLECTOR_PUSH_SOLENOID_MODULE, COLLECTOR_PULL_SOLENOID_MODULE),
m_climber(CLIMBER_MOTOR_ID, CLIMBER_PUSH_SOLENOID_MODULE, CLIMBER_PULL_SOLENOID_MODULE),
m_compressorController() {
    /*if (!handler.ready()) {
        // do something if handler failed to connect
    }*/

    m_flModule.setDrivePID(0.001, 0, 0);
    m_flModule.setSteerPID(2.0, 0, 0.01);

    m_frModule.setDrivePID(0.001, 0, 0);
    m_frModule.setSteerPID(2.0, 0, 0.01);

    m_blModule.setDrivePID(0.001, 0, 0);
    m_blModule.setSteerPID(2.0, 0, 0.01);

    m_brModule.setDrivePID(0.001, 0, 0);
    m_brModule.setSteerPID(2.0, 0, 0.01);

    m_swerveDrive.brakeOnStop();
}

void Robot::RobotInit() {
    frc::SmartDashboard::init();
    m_gyro.Init();
    m_gyro.Zero();
}

void Robot::RobotPeriodic() {
    m_gyro.Update();
    m_swerveDrive.updatePosition();

    frc::SmartDashboard::PutNumber("Distance Driven: ", m_swerveDrive.getAvgDistance());
    frc::SmartDashboard::PutNumber("Gyro Angle: ", m_gyro.getAngle());
    frc::SmartDashboard::PutNumber("Robot x Position", m_swerveDrive.getPosition().x());
    frc::SmartDashboard::PutNumber("Robot y Position", m_swerveDrive.getPosition().y());

    frc::SmartDashboard::PutBoolean("Slow speed enabled", m_slowSpeed);
    frc::SmartDashboard::PutBoolean("Collector Solenoid Toggle: ", m_collector.getValue());

    frc::SmartDashboard::PutNumber("Tank Pressure", m_compressorController.getTankPressure());

    frc::SmartDashboard::PutNumber("Current Shooter RPM", m_shooter.getSpeed());

    frc::SmartDashboard::PutNumber("Climber Arm Height (mm)", m_climber.getHeight());
    frc::SmartDashboard::PutNumber("Climber Arm Speed (mm/s)", m_climber.getSpeed());
    frc::SmartDashboard::PutNumber("Climber Arm Ticks Raised", m_climber.getTicks());
}

void Robot::DisabledInit() {
    m_doEnabledInit = true;
    m_swerveDrive.resetDrive();
}

void Robot::DisabledPeriodic() {
    m_autonomous.DecidePath();
    frc::SmartDashboard::PutString("Selected Autonomous", m_autonomous.GetCurrentPath());
}

void Robot::EnabledInit() {
    m_collector.resetSolenoid();
    m_climber.resetClimber();
}

void Robot::EnabledPeriodic() {
    m_compressorController.update();
}

void Robot::AutonomousInit() {
    if (m_doEnabledInit) {
        EnabledInit();
        m_doEnabledInit = false;
    }

    m_swerveDrive.unsetHoldAngle();
    m_gyro.Zero();
    m_autonomous.StartAuto();
}

void Robot::AutonomousPeriodic() {
    EnabledPeriodic();

    m_autonomous.PeriodicAuto();
}

void Robot::TeleopInit() {
    if (m_doEnabledInit) {
        EnabledInit();
        m_doEnabledInit = false;
    }
}

// main loop
void Robot::TeleopPeriodic() {
    EnabledPeriodic();

    /* =-=-=-=-=-=-=-=-=-=-= Climber Controls =-=-=-=-=-=-=-=-=-=-= */

    if (m_joystick.GetRawButton(DriverButton::Button8)) { // arms up
        m_climber.setArmSpeed(CLIMBER_MOTOR_PERCENT_OUTPUT);
    } else if (m_joystick.GetRawButton(DriverButton::Button7)) { // arms down
        m_climber.setArmSpeed(-CLIMBER_MOTOR_PERCENT_OUTPUT);
    } else {
        m_climber.setArmSpeed(0);
    }
    if (m_joystick.GetRawButtonPressed(DriverButton::Button6)) {
        m_climber.setSolenoid(true); // arms out
    }
    if (m_joystick.GetRawButtonPressed(DriverButton::Button4)) {
        m_climber.setSolenoid(false); // arms in
    }


    /* =-=-=-=-=-=-=-=-=-=-= Shooter Controls =-=-=-=-=-=-=-=-=-=-= */

    if (m_joystick.GetRawButton(DriverButton::ThumbButton)) {
        m_collector.setIndexerMotorSpeed(INDEXER_MOTOR_PERCENT_OUTPUT);
    } else {
        m_collector.setIndexerMotorSpeed(0);
    }
    if (m_joystick.GetRawButton(DriverButton::Button11)) {
        m_shooter.stop();
    }
    if (m_joystick.GetRawButton(DriverButton::Button12)) {
        m_shooter.setSpeed(SHOOTER_SPEED);
    }
    if (m_joystick.GetRawButton(DriverButton::Button9)) {
        m_shooter.setSpeed(LOW_SHOOTER_SPEED);
    }


    /* =-=-=-=-=-=-=-=-=-=- Collector Controls -=-=-=-=-=-=-=-=-=-= */

    if (m_xboxController.GetRawButtonPressed(OperatorButton::LeftBumper)) {
        m_collector.setSolenoid(false); // collecter in
    }
    if (m_xboxController.GetRawButtonPressed(OperatorButton::RightBumper)) {
        m_collector.setSolenoid(true); // collecter out
    }
    if (m_xboxController.GetRawButton(OperatorButton::Y)) {
        m_collector.setCollectorMotorSpeed(COLLECTOR_MOTOR_PERCENT_OUTPUT);
    } else if (m_xboxController.GetRawButton(OperatorButton::X)) {
        m_collector.setCollectorMotorSpeed(-COLLECTOR_MOTOR_PERCENT_OUTPUT);
    } else {
        m_collector.setCollectorMotorSpeed(0);
    }


    /* =-=-=-=-=-=-=-=-=-=-= Swerve Controls =-=-=-=-=-=-=-=-=-=-= */

    // calibrate gyro
    if (m_joystick.GetRawButton(DriverButton::Button5)) {
        m_gyro.Cal();
        m_gyro.Zero();
    }

    // dpad up for full speed,
    // down for half speed
    if (m_xboxController.GetPOV() == 0) {
        m_slowSpeed = false;
    } else if (m_xboxController.GetPOV() == 180) {
        m_slowSpeed = true;
    }

    float x = m_joystick.GetRawAxis(DriverJoystick::XAxis);
    float y = -1 * m_joystick.GetRawAxis(DriverJoystick::YAxis);
    float z = -1 * m_joystick.GetRawAxis(DriverJoystick::ZAxis);

    if (fabs(x) < 0.10) {
        x = 0;
    }
    if (fabs(y) < 0.10) {
        y = 0;
    }
    if (fabs(z) < 0.10) {
        z = 0;
    }

    bool inDeadzone = (sqrt(x * x + y * y) < JOYSTICK_DEADZONE) && (fabs(z) < JOYSTICK_DEADZONE); // Checks joystick deadzones

    // turn field oriented mode off if button 3 is pressed
    bool fieldOreo = !m_joystick.GetRawButton(DriverButton::Button3);

    if (!inDeadzone) {
        Vec2 directionVec(x, y);
        float rotationSpeed;
        SwerveDrive::UpdateMode mode = SwerveDrive::UpdateMode::Percent;

        if (m_slowSpeed) {
            directionVec *= SLOW_SPEED_MAX_SPEED;
            rotationSpeed = SLOW_SPEED_MAX_ROT_SPEED * z;
            mode = SwerveDrive::UpdateMode::Velocity;
        } else {
            rotationSpeed = 0.5 * z;
        }

        if (m_joystick.GetRawButton(DriverButton::Trigger)) {
            m_swerveDrive.swerveUpdate(directionVec, rotationSpeed, fieldOreo, mode);
        } else {
            m_swerveDrive.crabUpdate(directionVec, fieldOreo, mode);
        }
    } else {
        m_swerveDrive.stop();
    }

    // turn to the right angle for climbing
    if (m_joystick.GetRawButton(DriverButton::Button10)) {
        m_swerveDrive.turnToAngle(180.0);
    }
}

void Robot::TestInit() {
    TeleopInit();
    m_swerveDrive.setPosition(Vec2(0.0, 0.0));
    m_shooter.stop();
}

void Robot::TestPeriodic() {
    TeleopPeriodic();
}

int main() { return frc::StartRobot<Robot>(); } // Initiate main loop
