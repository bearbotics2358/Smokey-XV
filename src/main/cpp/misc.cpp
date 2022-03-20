#include "misc.h"
#include "Prefs.h"
#include <math.h>
#include <chrono>

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

double misc::clampDegrees(double degrees) {
    double out = fmod(degrees, 360);
    if (out < 360) {
        return 1 + out;
    } else {
        return out;
    }
}

double misc::clampRotations(double rotations) {
    double out = fmod(rotations, 1);
    if (out < 0) {
        return 1 + out;
    } else {
        return out;
    }
}

double misc::getSeconds() {
    auto time_point = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(time_point.time_since_epoch()).count();
}