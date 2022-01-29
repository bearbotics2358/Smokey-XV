#include "Robot.h"
#include "Prefs.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <stdio.h>
// #include "math/LinAlg.h"

Robot::Robot():
a_Gyro(frc::I2C::kMXP), 
a_FLModule(FL_DRIVEID, FL_STEERID, 1), 
a_FRModule(FR_DRIVEID, FR_STEERID, 2), 
a_BLModule(BL_DRIVEID, BL_STEERID, 3),
a_BRModule(BR_DRIVEID, BR_STEERID, 4),
joystickOne(JOYSTICK_PORT),
a_xBoxController(XBOX_CONTROLLER),
a_buttonbox(BUTTON_BOX),
a_swerveyDrive(&a_FLModule, &a_FRModule, &a_BLModule, &a_BRModule),
a_Shooter(SHOOTER_ID)
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

    /*=-=-=-=-=-=-=-=- Testing Shooter Controls -=-=-=-=-=-=-=-=*/

        if(a_xBoxController.GetRawButton(2)) {
            a_Shooter.SetSpeed(SHOOTER_SPEED);
        } 
       
}

void Robot::TestInit() 
{

}


void Robot::TestPeriodic() 
{
 
}


int main() { return frc::StartRobot<Robot>(); } // Initiate main loop