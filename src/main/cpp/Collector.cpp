#include "Collector.h"
#include "Prefs.h"
#include "misc.h"

Collector::Collector(int collectorMotorId, int indexerMotorId, int solenoidId, int pushSolenoidModule, int pullSolenoidModule):
a_collectorMotor(collectorMotorId),
a_indexerMotor(indexerMotorId),
a_collectorSolenoid(frc::PneumaticsModuleType::REVPH, pushSolenoidModule, pullSolenoidModule)
{

}

void Collector::toggleSolenoid() {
    a_collectorSolenoid.Toggle();
}

void Collector::resetSolenoid() {
    a_collectorSolenoid.Set(frc::DoubleSolenoid::Value::kForward);
}

void Collector::setCollectorMotorSpeed(double percent){
    a_collectorMotor.Set(ControlMode::PercentOutput, percent);
}
void Collector::setIndexerMotorSpeed(double percent){
    a_indexerMotor.Set(ControlMode::PercentOutput, percent);
}
bool Collector::getValue(){
    return a_collectorSolenoid.Get();
}