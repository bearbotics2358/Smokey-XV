
#include "Robot.h"
#include "Prefs.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <stdio.h>
#include "math/LinAlg.h"
#include <iostream>
/*~~ hi :) ~~ */
Robot::Robot():
a_Gyro(frc::I2C::kMXP), // 1
a_FLModule(FL_DRIVE_ID, FL_STEER_ID, 1), 
a_FRModule(FR_DRIVE_ID, FR_STEER_ID, 2), // (when we get analog encoders, replace 1-4 with actual IDs)
a_BLModule(BL_DRIVE_ID, BL_STEER_ID, 3),
a_BRModule(BR_DRIVE_ID, BR_STEER_ID, 4),
joystickOne(JOYSTICK_PORT),
a_xBoxController(XBOX_CONTROLLER),
a_buttonbox(BUTTON_BOX),
a_swerveyDrive(&a_FLModule, &a_FRModule, &a_BLModule, &a_BRModule),
// handler("169.254.179.144", "1185", "data"),
//handler("raspberrypi.local", 1883, "PI/CV/SHOOT/DATA"),
a_canHandler(CanHandler::layout2022())
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
    Vec2 tmp2(2.0f, 2.0f);
    Vec2 tmp3(2.0f, 2.0f);
    Vec4 tmp(0.0f, 1.0f, 2.0f, 3.0f);
    tmp3[1];
    tmp3.x();
    tmp3.y();
    tmp.w();
    tmp.magnitude();

    frc::SmartDashboard::init();
    a_Gyro.Init();
    // a_Gyro.Cal();
    a_Gyro.Zero();
}

void Robot::RobotPeriodic()
{
    a_Gyro.Update(); 
    // handler.update();

    frc::SmartDashboard::PutNumber("Distance Driven: ", a_swerveyDrive.getAvgDistance());
    frc::SmartDashboard::PutNumber("Gyro Angle: ", a_Gyro.GetAngle(0));

    a_canHandler.update();
    frc::SmartDashboard::PutNumber("Fl wheel angle", *a_canHandler.getData(FL_SWERVE_DATA_ID));
    frc::SmartDashboard::PutNumber("Fr wheel angle", *a_canHandler.getData(FR_SWERVE_DATA_ID));
    frc::SmartDashboard::PutNumber("Bl wheel angle", *a_canHandler.getData(BL_SWERVE_DATA_ID));
    frc::SmartDashboard::PutNumber("Br wheel angle", *a_canHandler.getData(BR_SWERVE_DATA_ID));
    printf("fl angle: %f\n", *a_canHandler.getData(FL_SWERVE_DATA_ID));
    printf("fr angle: %f\n", *a_canHandler.getData(FR_SWERVE_DATA_ID));
    printf("bl angle: %f\n", *a_canHandler.getData(BL_SWERVE_DATA_ID));
    printf("br angle: %f\n", *a_canHandler.getData(BR_SWERVE_DATA_ID));
    /*std::cout << "Front left angle" << *a_canHandler.getData(FL_SWERVE_DATA_ID);
    std::cout << "Front right angle" << *a_canHandler.getData(FR_SWERVE_DATA_ID);
    std::cout << "Back left angle" << *a_canHandler.getData(BL_SWERVE_DATA_ID);
    std::cout << "Back right angle" << *a_canHandler.getData(BR_SWERVE_DATA_ID); */
}

void Robot::DisabledInit()
{
    resetSwerveDrive();
}

void Robot::DisabledPeriodic()
{
}

void Robot::AutonomousInit() 
{
}

void Robot::AutonomousPeriodic() 
{
}

void Robot::TeleopInit() 
{

}

void Robot::TeleopPeriodic() // main loop
{

    /* =-=-=-=-=-=-=-=-=-=-= Joystick Controls =-=-=-=-=-=-=-=-=-=-= */

    float x = -1 * joystickOne.GetRawAxis(0);
    float y = -1 * joystickOne.GetRawAxis(1);
    float z = -1 * joystickOne.GetRawAxis(2);
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
    bool inDeadzone = (((sqrt(x * x + y * y) < JOYSTICK_DEADZONE) && (fabs(z) < JOYSTICK_DEADZONE)) ? true : false); // Checks joystick deadzones

    if(joystickOne.GetRawButton(5))
    {
        a_Gyro.Cal();
        a_Gyro.Zero();
    }

    if(joystickOne.GetRawButton(4))
    {
        // vision led on
    }
    else
    {
        // vision led off
    }
    if(joystickOne.GetRawButton(4)) // && has target (todo once written)
    {
        // track target with vision 
    }
    else if(joystickOne.GetRawButton(3))
    {
        if(!inDeadzone) 
        {
            if(joystickOne.GetRawButton(1)) 
            {
                a_swerveyDrive.swerveUpdate(x, y, 0.5 * z, gyro, false);
            } 
            else 
            {
                a_swerveyDrive.crabDriveUpdate(x, y, gyro);
            }
        } 
        else 
        {
            a_swerveyDrive.swerveUpdate(0, 0, 0, gyro, false);
        }
    }
    else
    {
        if(!inDeadzone) 
        {
            if(joystickOne.GetRawButton(1)) 
            {
                a_swerveyDrive.swerveUpdate(x, y, 0.5 * z, gyro, fieldOreo);
            } 
            else 
            {
                a_swerveyDrive.crabDriveUpdate(x, y, gyro);
            }
        } 
        else 
        {
            a_swerveyDrive.swerveUpdate(0, 0, 0, gyro, fieldOreo);
        }
    }
}

void Robot::TestInit() 
{

}


void Robot::TestPeriodic() 
{



    float x = -1 * joystickOne.GetRawAxis(0);
    float y = -1 * joystickOne.GetRawAxis(1);
    float z = -1 * joystickOne.GetRawAxis(2);
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
    bool inDeadzone = (((sqrt(x * x + y * y) < JOYSTICK_DEADZONE) && (fabs(z) < JOYSTICK_DEADZONE)) ? true : false); // Checks joystick deadzones




    if(joystickOne.GetRawButton(3)) {
        a_swerveyDrive.turnToAngle(gyro, 180.0);
    } else if(!inDeadzone) {
        if(joystickOne.GetRawButton(1)) 
        {
            if(joystickOne.GetRawButton(2)) 
            {
                // a_swerveyDrive.makeShiftTurn(a_LimeyLight.calcZAxis());
            } 
            else
            {
                a_swerveyDrive.swerveUpdate(x, y, 0.5 * z, gyro, fieldOreo);
            }
        } 
        else 
        
        {
           a_swerveyDrive.crabDriveUpdate(x, y, gyro);
        }
    } 
    else 
    {
        if(joystickOne.GetRawButton(2)) 
        {
            // a_swerveyDrive.makeShiftTurn(a_LimeyLight.calcZAxis());
        } 
        else
        {
            a_swerveyDrive.swerveUpdate(0, 0, 0, gyro, fieldOreo);
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

bool Robot::resetSwerveDrive() {
    auto flAngle = a_canHandler.getData(FL_SWERVE_DATA_ID);
    auto frAngle = a_canHandler.getData(FR_SWERVE_DATA_ID);
    auto blAngle = a_canHandler.getData(BL_SWERVE_DATA_ID);
    auto brAngle = a_canHandler.getData(BR_SWERVE_DATA_ID);

    if (flAngle.has_value() && frAngle.has_value() && blAngle.has_value() && brAngle.has_value()) {
        a_swerveyDrive.resetDrive(*flAngle, *frAngle, *blAngle, *brAngle);
        return true;
    } else {
        return false;
    }
}


int main() { return frc::StartRobot<Robot>(); } // Initiate main loop
