#pragma once

namespace misc {
// converts rpm to talon units per 100 ms
double rpmToTalonVel(double rpm);

// converts talon units per 100 ms to rpm
double talonVelToRpm(double units);

double degToRad(double degrees);
double radToDeg(double radians);

// clamps degrees to be in the range of 0-360
// if they are out of this range they are converted to an equivalent degrees in this range
double clampDegrees(double degrees);

double clampRotations(double rotations);
}