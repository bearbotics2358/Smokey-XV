#include "Collector.h"
#include "Prefs.h"
#include "misc.h"

Collector::Collector(int collectorMotorId, int indexerMotorId, int solenoidId):
a_collectorMotor(collectorMotorId),
a_indexerMotor(indexerMotorId),
a_collectorSolenoid(frc::PneumaticsModuleType::REVPH, 0, 1)
{

}

void Collector::toggleSolenoid() {
    a_collectorSolenoid.Toggle();
}

void Collector::resetSolenoid() {
    a_collectorSolenoid.Set(frc::DoubleSolenoid::Value::kReverse);
}

void Collector::setCollectorMotorSpeed(double rpm){
    double value = misc::rpmToTalonVel(rpm);
    a_collectorMotor.Set(ControlMode::Velocity, value);
}
void Collector::setIndexerMotorSpeed(double rpm){
    double value = misc::rpmToTalonVel(rpm);
    a_indexerMotor.Set(ControlMode::Velocity, value);
}
bool Collector::getValue(){
    return a_collectorSolenoid.Get();
}