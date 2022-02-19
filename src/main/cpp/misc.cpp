#include "misc.h"
#include "Prefs.h"
#include <math.h>

double misc::rpmToTalonVel(double rpm) {
    return (rpm * FALCON_UNITS_PER_REV) / 600.0;
}

double misc::talonVelToRpm(double units) {
    return (units * 600.0) / FALCON_UNITS_PER_REV;
}

double misc::degToRad(double degrees) {
    return degrees * 2 * M_PI / 360.0;
}

double misc::radToDeg(double radians) {
    return radians * 360 / (2 * M_PI);
}