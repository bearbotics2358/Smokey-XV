#include "BallShooter.h"
#include <algorithm>

#include "Prefs.h"

BallShooter::BallShooter(int leftId, int rightId): 
a_shooterLeft(rightId),
a_shooterRight(leftId)
{
        
}

void BallShooter::setSpeed(double rpm) {
    // to stop the motor breaking from going to fast instantly
    // smooths out desired rpm
    double input = (1 - alpha) * previousInput + alpha * rpm;
    previousInput = input;
        
    // with TalonFX::Set in VelocityMode, it wants a value which says
    // how many units to turn per 100 ms (0.1 sec)
    // there are 2048 of these units in 1 rotation
    // these units may or may not be encoder ticks, I didn't look to see
    double value = (input * FALCON_UNITS_PER_REV) / 600.0;

    a_shooterLeft.Set(ControlMode::Velocity, value);
    a_shooterRight.Set(ControlMode::Velocity, -value);
}

double BallShooter::getSpeed() {
    // NOTE: avergaing probably isn't necessary
    double leftValue = a_shooterLeft.GetSelectedSensorVelocity();
    double rightValue = a_shooterLeft.GetSelectedSensorVelocity();
    /*double avg = (leftValue + rightValue) / 2.0;*/
    // TEMP
    double avg = std::max(leftValue, rightValue);
    return (avg * 600.0) / FALCON_UNITS_PER_REV;
}


