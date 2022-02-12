#include "misc.h"
#include "Prefs.h"

double misc::rpmToTalonVel(double rpm) {
    return (rpm * FALCON_UNITS_PER_REV) / 600.0;
}

double misc::talonVelToRpm(double units) {
    return (units * 600.0) / FALCON_UNITS_PER_REV;
}