#pragma once
#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>
#include <frc/DigitalInput.h>

class Climber {
    public:
        Climber(int climberMotorId, int pushSolenoidModule, int pullSolenoidModule, int port);
        void setArmSpeed(double rpm);
        void toggleSolenoid();
        void resetClimber();
        double getHeight();
        double getSpeed();
        bool LifterZero();

    private:
        TalonFX a_climberArmMotor;
        frc::DoubleSolenoid a_climberSolenoid;
        frc::DigitalInput a_Switch;
};