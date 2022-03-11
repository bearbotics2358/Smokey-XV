#pragma once

#include "misc.h"
#include <ctre/Phoenix.h>
#include <rev/CANSparkMax.h>

/** Class for the shooter */
class BallShooter {
    public:
        /** Constructor for the shooter.
         *  @param leftId can id of left shooter motor.
         *  @param rightId can id of right shooter motor.
         */
        BallShooter(int leftId, int rightId);

        /** Set shooter speed in rpm.
         *  The requested input is slowly scaled up to match the input rpm, which avoids damaging the motor if it spools up too fast.
         *  Change the private member alpha to effect how fast this speeds up.
         *  @param rpm desired shooter speed in rpm.
         */
        void setSpeed(double rpm);

        /** Gets the current shooter speed in rpm. */
        double getSpeed();

    private:
        TalonFX a_shooterLeft;
        TalonFX a_shooterRight;

        // alpha and previous input are used to slowly scale the requested input to the desired speed,
        // instead of immedeately changing the requested value for the shooter, which could be a problem due to the heavy flywheels.
        double alpha { 0.5 };
        double previousInput { 0.0 };
};