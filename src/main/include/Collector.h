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
        bool getValue();


    private:

        TalonSRX a_collectorMotor;
        TalonSRX a_indexerMotor;
        frc::DoubleSolenoid a_collectorSolenoid;


        double alpha { 0.5 };
        double previousInput { 0.0 };

};