#include "SwerveDrive.h"

#include "math/ConstMath.h"
#include "misc.h"

#ifdef NEW_SWERVE
SwerveTransform::SwerveTransform(Vec2 direction, float rotSpeed):
direction(direction),
rotSpeed(rotSpeed) {}


SwerveDrive::SwerveDrive(SwerveModule& flModule, SwerveModule& frModule, SwerveModule& blModule, SwerveModule& brModule):
m_fl(flModule),
m_fr(frModule),
m_bl(blModule),
m_br(brModule),
m_anglePid(0.014, 0.0, 0.0) {}

SwerveDrive::~SwerveDrive() {}

void SwerveDrive::update(const SwerveTransform& transform) {
    constexpr float halfLength = DRIVE_LENGTH / 2;
    constexpr float halfWidth = DRIVE_WIDTH / 2;
    constexpr float driveRadius = constSqrt(halfLength * halfLength + halfWidth * halfWidth);

    constexpr Vec2 frPosVec = Vec2(halfWidth, halfLength);
    constexpr Vec2 flPosVec = Vec2(-halfWidth, halfLength);
    constexpr Vec2 brPosVec = Vec2(halfWidth, -halfLength);
    constexpr Vec2 blPosVec = Vec2(-halfWidth, -halfLength);

    constexpr Vec2 frTurn = frPosVec.right_normal().const_as_normalized();
    constexpr Vec2 flTurn = flPosVec.right_normal().const_as_normalized();
    constexpr Vec2 brTurn = brPosVec.right_normal().const_as_normalized();
    constexpr Vec2 blTurn = blPosVec.right_normal().const_as_normalized();

    float angularVelocity = transform.rotSpeed * driveRadius;

    Vec2 frVec = frTurn * angularVelocity + transform.direction;
    Vec2 flVec = flTurn * angularVelocity + transform.direction;
    Vec2 brVec = brTurn * angularVelocity + transform.direction;
    Vec2 blVec = blTurn * angularVelocity + transform.direction;

    m_fr.driveDirection(frVec);
    m_fl.driveDirection(flVec);
    m_br.driveDirection(brVec);
    m_bl.driveDirection(blVec);
}

float SwerveDrive::getAvgDistance() const {
    return (fabs(m_fr.getDistance()) + fabs(m_fl.getDistance()) + fabs(m_br.getDistance()) + fabs(m_bl.getDistance())) / 4.0;
}

#else

SwerveDrive::SwerveDrive(SwerveModule& flModule, SwerveModule& frModule, SwerveModule& blModule, SwerveModule& brModule):
flModule(flModule),
frModule(frModule),
blModule(blModule),
brModule(brModule),
turnAnglePid(0.014, 0.0, 0.0),
crabAnglePid(1.5, 0.0, 0.01) {
    turnAnglePid.EnableContinuousInput(0.0, 360.0);
    crabAnglePid.EnableContinuousInput(0.0, 360.0);
}

void SwerveDrive::crabDriveUpdate(float x, float y, float gyroDegrees, bool fieldOriented) {
    if (!crab) {
        holdAngle = gyroDegrees;
        crab = true;
    }

    auto z = std::clamp(crabAnglePid.Calculate(gyroDegrees, holdAngle) / 270.0, -0.5, 0.5);

    swerveUpdateInner(x, y, z, gyroDegrees, fieldOriented);
}

void SwerveDrive::swerveUpdate(float x, float y, float z, float gyroDegrees, bool fieldOriented) {
    crab = false;
    swerveUpdateInner(x, y, z, gyroDegrees, fieldOriented);
}

void SwerveDrive::unsetHoldAngle() {
    crab = false;
}

void SwerveDrive::resetDrive() {
    flModule.resetDriveEncoder();
    frModule.resetDriveEncoder();
    blModule.resetDriveEncoder();
    brModule.resetDriveEncoder();

    // flModule.resetSteerEncoder();
    // frModule.resetSteerEncoder();
    // blModule.resetSteerEncoder();
    // brModule.resetSteerEncoder();
}

void SwerveDrive::driveDistance(float distMeters, float directionDegrees) {
    flModule.steerToAng(directionDegrees);
    frModule.steerToAng(directionDegrees);
    blModule.steerToAng(directionDegrees);
    brModule.steerToAng(directionDegrees);

    flModule.goToPosition(distMeters);
    frModule.goToPosition(distMeters);
    blModule.goToPosition(distMeters);
    brModule.goToPosition(distMeters);
}

void SwerveDrive::driveDirection(float percent, float directionDegrees) {
    flModule.steerToAng(directionDegrees);
    frModule.steerToAng(directionDegrees);
    blModule.steerToAng(directionDegrees);
    brModule.steerToAng(directionDegrees);

    flModule.setDrivePercent(percent);
    frModule.setDrivePercent(percent);
    blModule.setDrivePercent(percent);
    brModule.setDrivePercent(percent);
}

float SwerveDrive::getAvgDistance() {
    return (fabs(flModule.getDistance()) + fabs(frModule.getDistance()) + fabs(blModule.getDistance()) + fabs(brModule.getDistance())) / 4.0;
}

void SwerveDrive::turnToAngle(float gyroDegrees, float angle) {
    gyroDegrees = misc::clampDegrees(gyroDegrees);
    // calculates a speed we need to go based off our current sensor and target position
    float speed = std::clamp(turnAnglePid.Calculate(gyroDegrees, angle), -0.2, 0.2);

    flModule.steerToAng(135);
    frModule.steerToAng(45);
    blModule.steerToAng(225);
    brModule.steerToAng(315);

    flModule.setDrivePercent(speed);
    frModule.setDrivePercent(speed);
    blModule.setDrivePercent(speed);
    brModule.setDrivePercent(speed);
}

void SwerveDrive::goToTheDon(float speed, float direction, float distance, float gyro, bool fieldOriented) {
    if (getAvgDistance() <= distance) {
        float radians = direction * M_PI / 180.0;

        float x = speed * sin(radians);
        float y = speed * cos(radians);

        crabDriveUpdate(x, y, gyro, fieldOriented);
    } else {
        swerveUpdate(0, 0, 0, gyro, false);
    }
}

void SwerveDrive::swerveUpdateInner(float x, float y, float z, float gyroDegrees, bool fieldOriented) {
    // Makes joystick inputs field oriented
    if (fieldOriented) {
        float gyroRadians = gyroDegrees * M_PI / 180;
        float temp = y * cos(gyroRadians) + x * sin(gyroRadians);
        x = -y * sin(gyroRadians) + x * cos(gyroRadians);
        y = temp;
    }

    float r = sqrt((DRIVE_LENGTH * DRIVE_LENGTH) + (DRIVE_WIDTH * DRIVE_WIDTH)); // radius of the drive base

    float a = x - z * (DRIVE_LENGTH / r); // temp variables to simplify math
    float b = x + z * (DRIVE_LENGTH / r);
    float c = y - z * (DRIVE_WIDTH / r);
    float d = y + z * (DRIVE_WIDTH / r);

    float flSpeed = sqrt(b * b + c * c);
    float frSpeed = sqrt(b * b + d * d);
    float blSpeed = sqrt(a * a + d * d);
    float brSpeed = sqrt(a * a + c * c);

    float flAngle = atan2(b, c) * 180 / M_PI; // calculates wheel angles and converts to radians
    float frAngle = atan2(b, d) * 180 / M_PI;
    float blAngle = atan2(a, c) * 180 / M_PI;
    float brAngle = atan2(a, d) * 180 / M_PI;

    if (flAngle < 0) {
        flAngle = flAngle + 360;
    }

    if (frAngle < 0) {
        frAngle = frAngle + 360;
    }

    if (blAngle < 0) {
        blAngle = blAngle + 360;
    }

    if (brAngle < 0) {
        brAngle = brAngle + 360;
    }

    float max = std::max(std::max(frSpeed, flSpeed), std::max(brSpeed, blSpeed)); // find max speed value

    // scale inputs respectively so no speed is greater than 1
    if (max > 1) {
        flSpeed /= max;
        frSpeed /= max;
        blSpeed /= max;
        brSpeed /= max;
    }

    float scalar = 1; // scalar to adjust if speed is too high
    flSpeed *= scalar;
    frSpeed *= scalar;
    blSpeed *= scalar;
    brSpeed *= scalar;

    float currentFL = flModule.getAngle();
    float currentFR = frModule.getAngle();
    float currentBR = brModule.getAngle();
    float currentBL = blModule.getAngle();

    float deadzoneCheck = sqrt(x * x + y * y);

    if (deadzoneCheck < 0.15 && fabs(z) < 0.01) {
        flSpeed = 0;
        frSpeed = 0;
        blSpeed = 0;
        brSpeed = 0;

        flAngle = currentFL;
        frAngle = currentFR;
        blAngle = currentBL;
        brAngle = currentBR;
    }

    // update speeds and angles
    if (flModule.adjustAngle(flAngle)) {
        flModule.setDrivePercent(-flSpeed);
    } else {
        flModule.setDrivePercent(flSpeed);
    }

    if (frModule.adjustAngle(frAngle)) {
        frModule.setDrivePercent(-frSpeed);
    } else {
        frModule.setDrivePercent(frSpeed);
    }

    if (blModule.adjustAngle(blAngle)) {
        blModule.setDrivePercent(-blSpeed);
    } else {
        blModule.setDrivePercent(blSpeed);
    }

    if (brModule.adjustAngle(brAngle)) {
        brModule.setDrivePercent(-brSpeed);
    } else {
        brModule.setDrivePercent(brSpeed);
    }
}

#endif