
#include "Robot.h"
#include "Prefs.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <stdio.h>
#include "math/LinAlg.h"
#include "buttons.h"
#include <iostream>
/*~~ hi :) ~~ */
Robot::Robot():
a_Gyro(frc::I2C::kMXP), // 1
a_FLModule(FL_DRIVE_ID, FL_STEER_ID, 1, FL_SWERVE_ABS_ENC_PORT, FL_SWERVE_ABS_ENC_OFFSET), 
a_FRModule(FR_DRIVE_ID, FR_STEER_ID, 2, FR_SWERVE_ABS_ENC_PORT, FR_SWERVE_ABS_ENC_OFFSET), // (when we get analog encoders, replace 1-4 with actual IDs)
a_BLModule(BL_DRIVE_ID, BL_STEER_ID, 3, BL_SWERVE_ABS_ENC_PORT, BL_SWERVE_ABS_ENC_OFFSET),
a_BRModule(BR_DRIVE_ID, BR_STEER_ID, 4, BR_SWERVE_ABS_ENC_PORT, BR_SWERVE_ABS_ENC_OFFSET),
joystickOne(JOYSTICK_PORT),
a_XboxController(XBOX_CONTROLLER),
a_buttonbox(BUTTON_BOX),
a_SwerveDrive(&a_FLModule, &a_FRModule, &a_BLModule, &a_BRModule),
a_Shooter(LEFT_SHOOTER_ID, RIGHT_SHOOTER_ID),
a_Collector(COLLECTOR_MOTOR_ID, SOLENOID_ID),
// handler("169.254.179.144", "1185", "data"),
//handler("raspberrypi.local", 1883, "PI/CV/SHOOT/DATA"),
//a_canHandler(CanHandler::layout2022()),
a_shooterVision(SHOOTER_CAMERA_NAME, TargetTracker::Mode::target(0)),
a_ballTracker(SHOOTER_CAMERA_NAME, TargetTracker::Mode::ball(0))
{
    /*if (!handler.ready()) {
        // do something if handler failed to connect
    }*/

    a_FLModule.updateDrivePID(0.001, 0, 0);
    a_FLModule.updateSteerPID(2.0, 0, 0.02);

    a_FRModule.updateDrivePID(0.001, 0, 0);
    a_FRModule.updateSteerPID(2.2, 0, 0.002);

    a_BLModule.updateDrivePID(0.001, 0, 0);
    a_BLModule.updateSteerPID(2.0, 0, 0.002);

    a_BRModule.updateDrivePID(0.001, 0, 0);
    a_BRModule.updateSteerPID(2.0, 0, 0.01);
}

void Robot::RobotInit() 
{
    frc::SmartDashboard::init();
    a_Gyro.Init();
    // a_Gyro.Cal();
    a_Gyro.Zero();
}

void Robot::RobotPeriodic()
{
    a_Gyro.Update(); 
    // handler.update();

    frc::SmartDashboard::PutNumber("Distance Driven: ", a_SwerveDrive.getAvgDistance());
    frc::SmartDashboard::PutNumber("Gyro Angle: ", a_Gyro.GetAngle(0));

    //a_canHandler.update();
    frc::SmartDashboard::PutNumber("Desired Shooter RPM", shooterDesiredSpeed);
    frc::SmartDashboard::PutNumber("Current Shooter RPM", a_Shooter.getSpeed());

    //frc::SmartDashboard::PutNumber("Fl wheel angle", *a_canHandler.getData(FL_SWERVE_DATA_ID));
    //frc::SmartDashboard::PutNumber("Fr wheel angle", *a_canHandler.getData(FR_SWERVE_DATA_ID));
    //frc::SmartDashboard::PutNumber("Bl wheel angle", *a_canHandler.getData(BL_SWERVE_DATA_ID));
    //frc::SmartDashboard::PutNumber("Br wheel angle", *a_canHandler.getData(BR_SWERVE_DATA_ID));
}

void Robot::DisabledInit()
{
    a_SwerveDrive.resetDrive();
    shooterDesiredSpeed = 0.0;
}

void Robot::DisabledPeriodic()
{
}

void Robot::AutonomousInit() 
{
}

void Robot::AutonomousPeriodic() 
{
    a_shooterVision.update();
    a_ballTracker.update();
}

void Robot::TeleopInit() 
{

}

void Robot::TeleopPeriodic() // main loop
{
    a_shooterVision.update();
    a_ballTracker.update();

    /* =-=-=-=-=-=-=-=-=-=-= Joystick Controls =-=-=-=-=-=-=-=-=-=-= */
    if(a_XboxController.GetRawButton(OperatorButton::A)) {
        shooterDesiredSpeed += 10.0;
    }
    if(a_XboxController.GetRawButton(OperatorButton::B)) {
        shooterDesiredSpeed -= 10.0;
    }

    /*=-=-=-=-=-=-=-=- Testing Collector Controls -=-=-=-=-=-=-=-=*/

    if(a_XboxController.GetRawButton(OperatorButton::X)){
        a_Collector.toggleSolenoid();
    }
    if(a_XboxController.GetRawButton(OperatorButton::Y)){
        a_Collector.setMotorSpeed(COLLECTOR_MOTOR_SPEED);
    }

    a_Shooter.setSpeed(shooterDesiredSpeed);

    float x = -1 * joystickOne.GetRawAxis(DriverJoystick::XAxis);
    float y = -1 * joystickOne.GetRawAxis(DriverJoystick::YAxis);
    float z = -1 * joystickOne.GetRawAxis(DriverJoystick::ZAxis);
    float gyro = a_Gyro.GetAngle(0);

    if(fabs(x) < 0.10)
    {
        x = 0;
    }
    if(fabs(y) < 0.10)
    {
        y = 0;
    }
    if(fabs(z) < 0.10)
    {
        z = 0;
    }
    
    if(gyro < 0)
    {
        gyro = fmod(gyro, -360);
        gyro += 360;
    }
    else
    {
        gyro = fmod(gyro, 360);
    }
    
    bool fieldOreo = true; // field oriented? - yes

    frc::SmartDashboard::PutNumber("Chase: ", z);
    bool inDeadzone = (sqrt(x * x + y * y) < JOYSTICK_DEADZONE) && (fabs(z) < JOYSTICK_DEADZONE); // Checks joystick deadzones

    if(joystickOne.GetRawButton(DriverButton::Button5))
    {
        a_Gyro.Cal();
        a_Gyro.Zero();
    }

    if(joystickOne.GetRawButton(DriverButton::Button4))
    {
        // vision led on
    }
    else
    {
        // vision led off
    }
    if(joystickOne.GetRawButton(DriverButton::Button4)) // && has target (todo once written)
    {
        // track target with vision 
    }
    else if(joystickOne.GetRawButton(DriverButton::Button3))
    {
        if(!inDeadzone) 
        {
            if(joystickOne.GetRawButton(DriverButton::Trigger)) 
            {
                a_SwerveDrive.swerveUpdate(x, y, 0.5 * z, gyro, false);
            } 
            else 
            {
                a_SwerveDrive.crabDriveUpdate(x, y, gyro);
            }
        } 
        else 
        {
            a_SwerveDrive.swerveUpdate(0, 0, 0, gyro, false);
        }
    }
    else
    {
        if(!inDeadzone) 
        {
            if(joystickOne.GetRawButton(DriverButton::Trigger)) 
            {
                a_SwerveDrive.swerveUpdate(x, y, 0.5 * z, gyro, fieldOreo);
            } 
            else 
            {
                a_SwerveDrive.crabDriveUpdate(x, y, gyro);
            }
        } 
        else 
        {
            a_SwerveDrive.swerveUpdate(0, 0, 0, gyro, fieldOreo);
        }
    }
}

void Robot::TestInit() 
{

}


void Robot::TestPeriodic() 
{



    float x = -1 * joystickOne.GetRawAxis(DriverJoystick::XAxis);
    float y = -1 * joystickOne.GetRawAxis(DriverJoystick::YAxis);
    float z = -1 * joystickOne.GetRawAxis(DriverJoystick::ZAxis);
    float gyro = a_Gyro.GetAngle(0);

    if(fabs(x) < 0.10)
    {
        x = 0;
    }
    if(fabs(y) < 0.10)
    {
        y = 0;
    }
    if(fabs(z) < 0.10)
    {
        z = 0;
    }
    
    if(gyro < 0)
    {
        gyro = fmod(gyro, -360);
        gyro += 360;
    }
    else
    {
        gyro = fmod(gyro, 360);
    }
    
    bool fieldOreo = true; // field oriented? - yes

    frc::SmartDashboard::PutNumber("Chase: ", z);
    bool inDeadzone = (sqrt(x * x + y * y) < JOYSTICK_DEADZONE) && (fabs(z) < JOYSTICK_DEADZONE); // Checks joystick deadzones




    if(joystickOne.GetRawButton(DriverButton::Button3)) {
        a_SwerveDrive.turnToAngle(gyro, 180.0);
    } else if(!inDeadzone) {
        if(joystickOne.GetRawButton(DriverButton::Trigger)) 
        {
            if(joystickOne.GetRawButton(DriverButton::ThumbButton)) 
            {
                // a_swerveyDrive.makeShiftTurn(a_LimeyLight.calcZAxis());
            } 
            else
            {
                a_SwerveDrive.swerveUpdate(x, y, 0.5 * z, gyro, fieldOreo);
            }
        } 
        else 
        
        {
           a_SwerveDrive.crabDriveUpdate(x, y, gyro);
        }
    } 
    else 
    {
        if(joystickOne.GetRawButton(DriverButton::ThumbButton)) 
        {
            // a_swerveyDrive.makeShiftTurn(a_LimeyLight.calcZAxis());
        } 
        else
        {
            a_SwerveDrive.swerveUpdate(0, 0, 0, gyro, fieldOreo);
        }
    }

    frc::SmartDashboard::PutNumber("Gyro: ", gyro);


    /* 

    if(fabs(a_xBoxController.GetRawAxis(1)) < 0.10) {
        a_CFS.ArmMove(0);
    } else {,
        a_CFS.ArmMove(a_xBoxController.GetRawAxis(1));
    }

    if(fabs(a_xBoxController.GetRawAxis(3)) < 0.10) {
        a_CFS.Collect(0);
    } else {
        a_CFS.Collect(a_xBoxController.GetRawAxis(3)); 
    }

    if(fabs(a_xBoxController.GetRawAxis(2)) < 0.10) {
        a_CFS.Collect(0);
    } else {
        a_CFS.Collect(-1.0 * a_xBoxController.GetRawAxis(2));
    }

    if(a_xBoxController.GetRawButton(6)) {
        a_CFS.Feed(FEED_SPEED);
    } else {
        a_CFS.Feed(0);
    }

    if(a_xBoxController.GetRawButton(5)) {
        a_CFS.Feed(-1.0 * FEED_SPEED); 
    } else {
        a_CFS.Feed(0);
    }

    if(a_xBoxController.GetPOV() == 0) {
        // climb
    } else {
        // don't climb
    }

    */ 


}

int main() { return frc::StartRobot<Robot>(); } // Initiate main loop
