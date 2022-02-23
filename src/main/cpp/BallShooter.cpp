#include "BallShooter.h"
#include <algorithm>

#include "Prefs.h"
#include "misc.h"

BallShooter::BallShooter(int leftId, int rightId):
a_shooterLeft(leftId),
a_shooterRight(rightId) {
    // by default this selects the ingetrated sensor
    ctre::phoenix::motorcontrol::can::TalonFXConfiguration config;

    // these settings are present in the documentation example, and since they relate to safety of motor, they are probably a good idea to include
    config.supplyCurrLimit.triggerThresholdCurrent = 40; // the peak supply current, in amps
    config.supplyCurrLimit.triggerThresholdTime = 1.5; // the time at the peak supply current before the limit triggers, in sec
    config.supplyCurrLimit.currentLimit = 30; // the current to maintain if the peak supply limit is triggered

    config.velocityMeasurementPeriod = ctre::phoenix::sensors::SensorVelocityMeasPeriod::Period_25Ms;

    // FIXME: pid tune
    config.slot0.kP = 0.4;

    a_shooterLeft.ConfigAllSettings(config);
    a_shooterRight.ConfigAllSettings(config);
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
    double value = misc::rpmToTalonVel(input);

    a_shooterLeft.Set(ControlMode::Velocity, value);
    a_shooterRight.Set(ControlMode::Velocity, -value);
}

double BallShooter::getSpeed() {
    // NOTE: avergaing probably isn't necessary
    double leftValue = a_shooterLeft.GetSelectedSensorVelocity();
    double rightValue = a_shooterRight.GetSelectedSensorVelocity();
    double avg = (leftValue - rightValue) / 2.0;
    return misc::talonVelToRpm(avg);
}