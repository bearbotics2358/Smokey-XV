#include "Climber.h"
#include "misc.h"
#include "Prefs.h"

Climber::Climber(int climberMotorId, int pushSolenoidModule, int pullSolenoidModule):
a_climberArmMotor(climberMotorId),
a_climberSolenoid(frc::PneumaticsModuleType::REVPH, pushSolenoidModule, pullSolenoidModule)
{
    a_climberArmMotor.SetNeutralMode(NeutralMode::Brake);
    a_climberArmMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);
    a_climberArmMotor.GetSensorCollection().SetIntegratedSensorPosition(0, 0);
}

void Climber::setArmSpeed(double rpm) {
    double value = misc::rpmToTalonVel(rpm);
    a_climberArmMotor.Set(ControlMode::Velocity, value);
}

void Climber::toggleSolenoid() {
    a_climberSolenoid.Toggle();
}

void Climber::resetSolenoid() {
    a_climberSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
}

double Climber::getHeight() { // in mm
    double ticks = a_climberArmMotor.GetSensorCollection().GetIntegratedSensorPosition() * 4; // multiplied by 4 because the encoder is QuadEncoder
    return ticks * CLIMBER_MM_PER_TICK;
}