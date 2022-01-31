#include "BallShooter.h"

    BallShooter::BallShooter(int id): 
    a_Shooter(SHOOTER_ID)
    {

    }

    void BallShooter::SetSpeed(double speed) {

        a_Shooter.Set(ControlMode::PercentOutput, speed);

    }


