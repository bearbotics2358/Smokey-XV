#include "Robot.h"
#include "Prefs.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <stdio.h>
#include "math/LinAlg.h"

Robot::Robot():
a_Gyro(frc::I2C::kMXP), // 1
a_FLModule(FL_DRIVE_ID, FL_STEER_ID, 1), 
a_FRModule(FR_DRIVE_ID, FR_STEER_ID, 2), 
a_BLModule(BL_DRIVE_ID, BL_STEER_ID, 3),
a_BRModule(BR_DRIVE_ID, BR_STEER_ID, 4),
joystickOne(JOYSTICK_PORT),
a_xBoxController(XBOX_CONTROLLER),
a_buttonbox(BUTTON_BOX),
a_swerveyDrive(&a_FLModule, &a_FRModule, &a_BLModule, &a_BRModule),
a_LimeyLight(),
a_CFS(SHOOT_1, SHOOT_2, FEED_1, FEED_2, COLLECT, PIVOT, CLIMBER, BROKEN_BEAM, REESES_BEAM),
a_JAutonomous(&a_Gyro, &a_buttonbox, &a_swerveyDrive, &a_CFS, &a_LimeyLight)
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

    a_LimeyLight.ledOff();
    a_LimeyLight.cameraMode(0);
}

void Robot::RobotPeriodic()
{
    a_Gyro.Update(); 
    // handler.update();
    frc::SmartDashboard::PutNumber("Wheel Speed L: ", a_CFS.GetWheelSpeedL());
    frc::SmartDashboard::PutNumber("Wheel Speed R: ", a_CFS.GetWheelSpeedR());

    frc::SmartDashboard::PutBoolean("Bottom Beam Break: ", a_CFS.GetBottomBeam());
    frc::SmartDashboard::PutBoolean("Top Beam Break: ", a_CFS.GetTopBeam());
    
    // frc::SmartDashboard::PutNumber("Pivot Voltage: ", a_CFS.GetPivotPosition());
    frc::SmartDashboard::PutNumber("Pivot Angle: ", a_CFS.GetArmAngle());
    frc::SmartDashboard::PutBoolean("Limelight Target?", a_LimeyLight.isTarget());
    frc::SmartDashboard::PutNumber("Distance Limelight: ", a_LimeyLight.getDist());

    // frc::SmartDashboard::PutNumber("Feeder Top: ", a_CFS.GetFeedSpeedTop());
    // frc::SmartDashboard::PutNumber("Feeder Bot: ", a_CFS.GetFeedSpeedBot());

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

    if(joystickOne.GetRawButton(4))
    {
        a_LimeyLight.ledOn();
    }
    else
    {
        a_LimeyLight.ledOff();
    }
    if(joystickOne.GetRawButton(4) && a_LimeyLight.isTarget()) // can see a target and toggled on
    {
        if(!inDeadzone) 
        {
            a_swerveyDrive.swerveUpdate(x, y, a_LimeyLight.calcZAxis(), gyro, true);
        }
        else
        {
            a_swerveyDrive.swerveUpdate(0, 0, a_LimeyLight.calcZAxis(), gyro, true);
        }   
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
                a_swerveyDrive.makeShiftTurn(a_LimeyLight.calcZAxis());
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
            a_swerveyDrive.makeShiftTurn(a_LimeyLight.calcZAxis());
        } 
        else
        {
            a_swerveyDrive.swerveUpdate(0, 0, 0, gyro, fieldOreo);
        }
    }

    frc::SmartDashboard::PutNumber("Gyro: ", gyro);

}


int main() { return frc::StartRobot<Robot>(); } // Initiate main loop