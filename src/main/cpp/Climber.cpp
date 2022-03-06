#include "Climber.h"
#include "misc.h"
#include "Prefs.h"

Climber::Climber(int climberMotorId, int pushSolenoidModule, int pullSolenoidModule):
a_climberArmMotor(climberMotorId),
a_climberSolenoid(frc::PneumaticsModuleType::REVPH, pushSolenoidModule, pullSolenoidModule)
{
    a_climberArmMotor.SetNeutralMode(NeutralMode::Brake);
    a_climberArmMotor.ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 0);

}

void Climber::setArmSpeed(double mmPerSecond) {
    double value = (mmPerSecond * 0.1) * CLIMBER_TICKS_PER_MM;
    a_climberArmMotor.Set(ControlMode::Velocity, value);
}

void Climber::toggleSolenoid() {
    a_climberSolenoid.Toggle();
}

void Climber::resetClimber() {
    a_climberSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
    a_climberArmMotor.GetSensorCollection().SetIntegratedSensorPosition(0, 0);
}

double Climber::getHeight() { // in mm
    double ticks = a_climberArmMotor.GetSensorCollection().GetIntegratedSensorPosition();
    return ticks * CLIMBER_MM_PER_TICK;
}
double Climber::getSpeed() { // in mm/s
    double ticks = a_climberArmMotor.GetSensorCollection().GetIntegratedSensorVelocity();
    return ticks * CLIMBER_MM_PER_TICK * 10;
}
