#include "Collector.h"

Collector::Collector(int motorId, int rightSolenoidId, int leftSolenoidId):
a_collectorMotor(motorId),
a_rightSolenoid(rightSolenoidId, frc::PneumaticsModuleType::REVPH, 0, 1),
a_leftSolenoid(leftSolenoidId, frc::PneumaticsModuleType::REVPH, 0, 1)
{

}