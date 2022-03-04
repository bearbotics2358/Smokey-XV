#include "Climber.h"
#include "misc.h"

Climber::Climber(int climberMotorId, int pushSolenoidModule, int pullSolenoidModule):
a_climberBackArmMotor(climberMotorId),
a_climberSolenoid(frc::PneumaticsModuleType::REVPH, pushSolenoidModule, pullSolenoidModule)
{
     
}

void Climber::setArmSpeed(double rpm) {
    double value = misc::rpmToTalonVel(rpm);
    a_climberBackArmMotor.Set(ControlMode::Velocity, value);
}