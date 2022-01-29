#include "Robot.h"
#include "Prefs.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <stdio.h>
// #include "math/LinAlg.h"

Robot::Robot():
a_Gyro(frc::I2C::kMXP), // 1
a_FLModule(FL_DRIVEID, FL_STEERID, 1), 
a_FRModule(FR_DRIVEID, FR_STEERID, 2), 
a_BLModule(BL_DRIVEID, BL_STEERID, 3),
a_BRModule(BR_DRIVEID, BR_STEERID, 4),
joystickOne(JOYSTICK_PORT),
a_xBoxController(XBOX_CONTROLLER),
a_buttonbox(BUTTON_BOX),
a_swerveyDrive(&a_FLModule, &a_FRModule, &a_BLModule, &a_BRModule)
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
  /* 
    Vec2 tmp2(2.0f, 2.0f);
    Vec2 tmp3(2.0f, 2.0f);
    Vec4 tmp(0.0f, 1.0f, 2.0f, 3.0f);
    tmp3[1];
    tmp3.x();
    tmp3.y();
    tmp.w();
    tmp.magnitude();
  */ 
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
}

void Robot::DisabledInit()
{
    a_swerveyDrive.resetDrive();
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

    
  if(joystickOne.GetRawButton(3))
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

}


int main() { return frc::StartRobot<Robot>(); } // Initiate main loop