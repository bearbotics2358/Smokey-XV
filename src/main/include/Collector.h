#pragma once

#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>

class Collector {
    public:

        Collector(int collectorMotorId, int indexerMotorId, int solenoidId);

        void toggleSolenoid();
        void resetSolenoid();
        void setCollectorMotorSpeed(double rpm);
        void setIndexerMotorSpeed(double rpm);
        static frc::DoubleSolenoid a_collectorSolenoid;


    private:

        TalonSRX a_collectorMotor;
        TalonSRX a_indexerMotor;


        double alpha { 0.5 };
        double previousInput { 0.0 };

};