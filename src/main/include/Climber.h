#pragma once
#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>

class Climber {
    public:
        Climber(int climberMotorId, int pushSolenoidModule, int pullSolenoidModule);
        void setArmSpeed(double percent);
        void toggleSolenoid();
        void resetClimber();
        double getHeight();
        double getTicks();
        double getSpeed();

    private:
        TalonFX a_climberArmMotor;
        frc::DoubleSolenoid a_climberSolenoid;
};