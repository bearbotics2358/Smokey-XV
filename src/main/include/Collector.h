#pragma once

#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>

class Collector {
    public:

    Collector(int motorId, int rightSolenoidId, int leftSolenoidId);

    private:

    TalonFX a_collectorMotor;
    frc::DoubleSolenoid a_rightSolenoid;
    frc::DoubleSolenoid a_leftSolenoid;

};