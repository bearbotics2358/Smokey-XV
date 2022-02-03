#include "BallShooter.h"

    BallShooter::BallShooter(int leftId, int rightId): 
    a_ShooterControllerLeft(leftId),
    a_ShooterControllerRight(rightId)
    {
        
    }

    void BallShooter::SetSpeed(double speed) {

        a_ShooterControllerLeft.Set(ControlMode::PercentOutput, speed);
        a_ShooterControllerRight.Set(ControlMode::PercentOutput, -speed);

    }


