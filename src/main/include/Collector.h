#pragma once

#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>

class Collector {
    public:

    Collector(int motorId, int solenoidId);

    void toggleSolenoid();
    void resetSolenoid();
    void setMotorSpeed(double rpm);


    private:

    TalonFX a_collectorMotor;
    frc::DoubleSolenoid a_collectorSolenoid;

    double alpha { 0.5 };
    double previousInput { 0.0 };

};