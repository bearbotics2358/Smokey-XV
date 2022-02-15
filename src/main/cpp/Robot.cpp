#include "Robot.h"
#include "Prefs.h"
#include <memory>
#include <frc/smartdashboard/SmartDashboard.h>
#include <stdio.h>
#include <wpi/StringMap.h>
// #include "math/LinAlg.h"

Robot::Robot():
#if 0
a_Gyro(frc::I2C::kMXP), 
a_FLModule(FL_DRIVEID, FL_STEERID, 1), 
a_FRModule(FR_DRIVEID, FR_STEERID, 2), 
a_BLModule(BL_DRIVEID, BL_STEERID, 3),
a_BRModule(BR_DRIVEID, BR_STEERID, 4),
a_Joystick(JOYSTICK_PORT),
a_SwerveDrive(&a_FLModule, &a_FRModule, &a_BLModule, &a_BRModule),
#endif
a_XboxController(XBOX_CONTROLLER),
a_Shooter(LEFT_SHOOTER_ID, RIGHT_SHOOTER_ID),
a_Collector(COLLECTOR_MOTOR_ID, SOLENOID_ID)
/*a_mainTab(frc::Shuffleboard::GetTab("Main Tab")),
a_setRpm(a_mainTab.Add("Desired RPM", 0.0)
    .WithWidget(frc::BuiltInWidgets::kNumberSlider)
    .GetEntry()),
a_currentRpm(
    a_mainTab.Add("Current RPM", 0.0)
    .WithWidget(frc::BuiltInWidgets::kTextView)
    .WithProperties(
        wpi::StringMap<std::shared_ptr<nt::Value>>({
            {"Min", nt::Value::MakeDouble(-5000.0)},
            {"Max", nt::Value::MakeDouble(5000)}
        })
    )
    .GetEntry()),
a_alpha(a_mainTab.Add("Alpha", 0.5)
    .WithWidget(frc::BuiltInWidgets::kNumberSlider)
    .WithProperties(
        wpi::StringMap<std::shared_ptr<nt::Value>>({
            {"Min", nt::Value::MakeDouble(0.0)},
            {"Max", nt::Value::MakeDouble(1.0)}
        })
    )
    .GetEntry())
*/
{
    /*if (!handler.ready()) {
        // do something if handler failed to connect
    }*/

    /* 
    a_FLModule.updateDrivePID(0.001, 0, 0);
    a_FLModule.updateSteerPID(2.0, 0, 0.02);

    a_FRModule.updateDrivePID(0.001, 0, 0);
    a_FRModule.updateSteerPID(2.2, 0, 0.002);

    a_BLModule.updateDrivePID(0.001, 0, 0);
    a_BLModule.updateSteerPID(2.0, 0, 0.002);

    a_BRModule.updateDrivePID(0.001, 0, 0);
    a_BRModule.updateSteerPID(2.0, 0, 0.01);
    */ 
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
    #if 0
    a_Gyro.Init();
    // a_Gyro.Cal();
    a_Gyro.Zero();
    #endif

}

void Robot::RobotPeriodic()
{
    #if 0
    a_Gyro.Update();
    #endif 
    // handler.update();

    //frc::SmartDashboard::PutNumber("Distance Driven: ", a_SwerveDrive.getAvgDistance());
    //frc::SmartDashboard::PutNumber("Gyro Angle: ", a_Gyro.GetAngle(0));
    frc::SmartDashboard::PutNumber("Desired Shooter RPM", shooterDesiredSpeed);
    frc::SmartDashboard::PutNumber("Current Shooter RPM", a_Shooter.getSpeed());
}

void Robot::DisabledInit()
{
    #if 0
    a_SwerveDrive.resetDrive();
    #endif
    shooterDesiredSpeed = 500.0;
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

        if(a_XboxController.GetRawButton(1)) {
            shooterDesiredSpeed += 10.0;
        }
        if(a_XboxController.GetRawButton(2)) {
            shooterDesiredSpeed -= 10.0;
        }

       //a_Shooter.setAlpha(a_alpha.GetDouble(0.5));
       //a_Shooter.setSpeed(a_setRpm.GetDouble(0.0));
       a_Shooter.setSpeed(shooterDesiredSpeed);
       //a_currentRpm.SetDouble(a_Shooter.getSpeed());

       /*
        if(a_xBoxController.GetRawAxis(1)) {
            a_Shooter.SetSpeed(a_XboxController.GetRawAxis(1));
        }
       */

    /*=-=-=-=-=-=-=-=- Testing Collector Controls -=-=-=-=-=-=-=-=*/

        if(a_XboxController.GetRawButton(3)){
            a_Collector.toggleSolenoid();
        }
        if(a_XboxController.GetRawButton(4)){
            a_Collector.setMotorSpeed(120);
        }
}

void Robot::TestInit() 
{

}


void Robot::TestPeriodic() 
{
 
}


int main() { return frc::StartRobot<Robot>(); } // Initiate main loop