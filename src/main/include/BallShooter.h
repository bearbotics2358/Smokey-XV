#pragma once

#include <ctre/Phoenix.h>
#include <rev/CANSparkMax.h>

class BallShooter {
    public:
        BallShooter(int leftId, int rightId);

        // sets speed in rpm
        void setSpeed(double rpm);
        // gets speed in rpms
        double getSpeed();

        // TEMP
        void setAlpha(double alpha) { this->alpha = alpha; }

    private:
        TalonFX a_shooterLeft;
        TalonFX a_shooterRight;

        double alpha { 0.5 };
        double previousInput { 0.0 };
};