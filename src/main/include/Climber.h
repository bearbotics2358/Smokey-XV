#pragma once
#include <frc/DoubleSolenoid.h>
#include <ctre/Phoenix.h>

class Climber {
    public:
        Climber(int climberMotorId, int pushSolenoidModule, int pullSolenoidModule);
        void setArmSpeed(double rpm);
        void toggleSolenoid();
        void resetClimber();
        double getHeight();
        double getSpeed();

    private:
        frc::DoubleSolenoid a_climberSolenoid;
        TalonFX a_climberArmMotor;
};