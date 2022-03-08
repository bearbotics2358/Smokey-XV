#pragma once
#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>

class Climber {
    public:
        Climber(int climberMotorId, int pushSolenoidModule, int pullSolenoidModule);
        void setArmSpeed(double rpm);
        void toggleSolenoid();
        void resetClimber();
        double getHeight();
        double getSpeed();

    private:
        TalonFX a_climberArmMotor;
        frc::DoubleSolenoid a_climberSolenoid;
};