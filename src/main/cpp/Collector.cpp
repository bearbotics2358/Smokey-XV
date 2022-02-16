#include "Collector.h"
#include "Prefs.h"

Collector::Collector(int motorId, int solenoidId):
a_collectorMotor(motorId),
a_collectorSolenoid(solenoidId, frc::PneumaticsModuleType::REVPH, 0, 1)
{

}
void Collector::toggleSolenoid(){
    a_collectorSolenoid.Toggle();
}

void Collector::resetSolenoid(){
    a_collectorSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
}

void Collector::setMotorSpeed(double rpm){
    // copied from the BallShooter.cpp setSpeed method, since it's the exact same thing
    double input = (1 - alpha) * previousInput + alpha * rpm;
    previousInput = input;

    double value = (input * FALCON_UNITS_PER_REV) / 600.0;
    a_collectorMotor.Set(ControlMode::Velocity, value);
}