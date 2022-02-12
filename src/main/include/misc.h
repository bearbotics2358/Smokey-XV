#pragma once

namespace misc {
    // converts rpm to talon units per 100 ms
    double rpmToTalonVel(double rpm);

    // converts talon units per 100 ms to rpm
    double talonVelToRpm(double units);
}