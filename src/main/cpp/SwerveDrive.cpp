#include "SwerveDrive.h"

#include "math/ConstMath.h"
#include "misc.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <math.h>

SwerveDrive::SwerveDrive(SwerveModule& flModule, SwerveModule& frModule, SwerveModule& blModule, SwerveModule& brModule, JrimmyGyro& gyro):
m_fl(flModule),
m_fr(frModule),
m_bl(blModule),
m_br(brModule),
m_gyro(gyro),
m_turnAnglePid(0.014, 0.0, 0.0),
m_strafeAnglePid(1.5, 0.0, 0.01) {
    m_turnAnglePid.EnableContinuousInput(0.0, 360.0);
    m_strafeAnglePid.EnableContinuousInput(0.0, 360.0);
}

void SwerveDrive::strafeUpdate(Vec2 direction, bool fieldOriented, UpdateMode mode) {
    float gyroDegrees = m_gyro.getAngleClamped();

    if (!m_strafe) {
        m_holdAngle = gyroDegrees;
        m_strafe = true;
    }

    swerveUpdateInner(direction, strafeCalcZ(m_holdAngle, gyroDegrees, mode), gyroDegrees, fieldOriented, mode);
}

void SwerveDrive::swerveUpdate(Vec2 direction, float rotationSpeed, bool fieldOriented, UpdateMode mode) {
    m_strafe = false;
    swerveUpdateInner(direction, rotationSpeed, m_gyro.getAngleClamped(), fieldOriented, mode);
}

void SwerveDrive::stop() {
    m_fl.setDrivePercent(0.0);
    m_fr.setDrivePercent(0.0);
    m_bl.setDrivePercent(0.0);
    m_br.setDrivePercent(0.0);

    m_fl.setSteerPercent(0.0);
    m_fr.setSteerPercent(0.0);
    m_bl.setSteerPercent(0.0);
    m_br.setSteerPercent(0.0);
}

void SwerveDrive::brakeOnStop() {
    m_fl.setBrakeMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_fr.setBrakeMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_bl.setBrakeMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
    m_br.setBrakeMode(ctre::phoenix::motorcontrol::NeutralMode::Brake);
}

void SwerveDrive::coastOnStop() {
    m_fl.setBrakeMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
    m_fr.setBrakeMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
    m_bl.setBrakeMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
    m_br.setBrakeMode(ctre::phoenix::motorcontrol::NeutralMode::Coast);
}

void SwerveDrive::setHoldAngle(float degrees) {
    m_strafe = true;
    m_holdAngle = degrees;
}

void SwerveDrive::unsetHoldAngle() {
    m_strafe = false;
}

void SwerveDrive::resetDrive() {
    m_fl.resetDriveEncoder();
    m_fr.resetDriveEncoder();
    m_bl.resetDriveEncoder();
    m_br.resetDriveEncoder();

    // flModule.resetSteerEncoder();
    // frModule.resetSteerEncoder();
    // blModule.resetSteerEncoder();
    // brModule.resetSteerEncoder();
}

void SwerveDrive::driveDistance(float distMeters, float directionDegrees) {
    m_fl.steerToAng(directionDegrees);
    m_fr.steerToAng(directionDegrees);
    m_bl.steerToAng(directionDegrees);
    m_br.steerToAng(directionDegrees);

    m_fl.goToPosition(distMeters);
    m_fr.goToPosition(distMeters);
    m_bl.goToPosition(distMeters);
    m_br.goToPosition(distMeters);
}

void SwerveDrive::driveDirection(float percent, float directionDegrees) {
    m_fl.steerToAng(directionDegrees);
    m_fr.steerToAng(directionDegrees);
    m_bl.steerToAng(directionDegrees);
    m_br.steerToAng(directionDegrees);

    m_fl.setDrivePercent(percent);
    m_fr.setDrivePercent(percent);
    m_bl.setDrivePercent(percent);
    m_br.setDrivePercent(percent);
}

float SwerveDrive::getAvgDistance() {
    return (fabs(m_fl.getDistance()) + fabs(m_fr.getDistance()) + fabs(m_bl.getDistance()) + fabs(m_br.getDistance())) / 4.0;
}

void SwerveDrive::turnToAngle(float angle) {
    float gyroDegrees = m_gyro.getAngleClamped();
    // calculates a speed we need to go based off our current sensor and target position
    float speed = turnCalcZ(angle, gyroDegrees);

    m_fl.steerToAng(135);
    m_fr.steerToAng(45);
    m_bl.steerToAng(225);
    m_br.steerToAng(315);

    m_fl.setDrivePercent(speed);
    m_fr.setDrivePercent(speed);
    m_bl.setDrivePercent(speed);
    m_br.setDrivePercent(speed);
}

void SwerveDrive::goToTheDon(float speed, float direction, float distance, bool fieldOriented) {
    if (getAvgDistance() <= distance) {
        float radians = direction * M_PI / 180.0;

        float x = speed * -sin(radians);
        float y = speed * cos(radians);

        strafeUpdate(Vec2(x, y), fieldOriented);
    } else {
        stop();
    }
}

bool SwerveDrive::goToPosition(Vec2 position, float degrees, float maxSpeed) {
    float gyroDegrees = m_gyro.getAngleClamped();
    auto relPosVector = position - m_position;
    float remainingDistance = relPosVector.magnitude();

    // create a unit vector pointing towards the point we want to go to
    auto directionVector = relPosVector.as_normalized();

    // the desired speed to go at
    // do it as sqrt of remaining distance since kinetic energy is proportional to velocity squared,
    // so with sqrt we will stop at the end
    // tune the constant in front of remainingDistance
    float speed = sqrt(0.35 * remainingDistance);

    // scale this vector by the requested speed, and slow down as we get closer to the target
    directionVector *= std::clamp((double) std::min(speed, maxSpeed), 0.0, 1.0);

    // flip sign of x because x is inverted for swerveUpdateInner
    swerveUpdateInner(directionVector, turnCalcZ(degrees, gyroDegrees), gyroDegrees, true, UpdateMode::Percent);

    return remainingDistance < GO_TO_DIST_DONE && misc::degreesDiff(degrees, gyroDegrees) < GO_TO_ANGLE_DONE;
}

void SwerveDrive::updatePosition() {
    // get the change in position of each wheel
    float flPos = m_fl.getDistance();
    float flPosChange = flPos - m_flLastPos;
    m_flLastPos = flPos;

    float frPos = m_fr.getDistance();
    float frPosChange = frPos - m_frLastPos;
    m_frLastPos = frPos;

    float blPos = m_bl.getDistance();
    float blPosChange = blPos - m_blLastPos;
    m_blLastPos = blPos;

    float brPos = m_br.getDistance();
    float brPosChange = brPos - m_brLastPos;
    m_brLastPos = brPos;

    // angle does not need to be clamped for creating the vector
    float gyroDegrees = m_gyro.getAngle();
    // TODO: figure out if angle for swerve turn motors is clockwise our counterclockwise
    // these angles are with 0 radians pointing in the direction of positive y, and they go counterclockwise
    float flAngle = misc::degToRad(m_fl.getAngle() + gyroDegrees);
    float frAngle = misc::degToRad(m_fr.getAngle() + gyroDegrees);
    float blAngle = misc::degToRad(m_bl.getAngle() + gyroDegrees);
    float brAngle = misc::degToRad(m_br.getAngle() + gyroDegrees);

    // create unit vectors pointing in the direction of the wheels
    Vec2 flVec(-sin(flAngle), cos(flAngle));
    Vec2 frVec(-sin(frAngle), cos(frAngle));
    Vec2 blVec(-sin(blAngle), cos(blAngle));
    Vec2 brVec(-sin(brAngle), cos(brAngle));

    // scale the wheel vector by the position change of each wheel
    flVec *= flPosChange;
    frVec *= frPosChange;
    blVec *= blPosChange;
    brVec *= brPosChange;

    // get the position change vector
    // adding all the vectors together will cancel out the turn part, leaving just the movement part * 4, so divide by 4
    // this doesn't check if the vectors are a valid wheel configuration for swerve to work, but the drive commands ensure this should work
    // if it isn't a valid configuration, some wheels will be slipping, so we can't predict the position anyways
    m_position += (flVec + frVec + blVec + brVec) / 4.0;
}

Vec2 SwerveDrive::getPosition() const {
    return m_position;
}

void SwerveDrive::setPosition(Vec2 position) {
    m_position = position;
}

void SwerveDrive::swerveUpdateInner(Vec2 direction, float rotationSpeed, float gyroDegrees, bool fieldOriented, UpdateMode mode) {
    if (fieldOriented) {
        direction = Mat2::rotation(misc::degToRad(-gyroDegrees)) * direction;
    }

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

    // TODO: rotationSpeed might need to be scaled by a constant in order to feel right
    if (mode == UpdateMode::Velocity) {
        rotationSpeed = misc::degToRad(rotationSpeed);
    }

    float linearTurnVelocity = rotationSpeed * driveRadius;

    Vec2 frVec = frTurn * linearTurnVelocity + direction;
    Vec2 flVec = flTurn * linearTurnVelocity + direction;
    Vec2 brVec = brTurn * linearTurnVelocity + direction;
    Vec2 blVec = blTurn * linearTurnVelocity + direction;

    if (mode == UpdateMode::Velocity) {
        m_fr.driveDirectionVelocity(frVec);
        m_fl.driveDirectionVelocity(flVec);
        m_br.driveDirectionVelocity(brVec);
        m_bl.driveDirectionVelocity(blVec);
    } else {
        m_fr.driveDirectionPercent(frVec);
        m_fl.driveDirectionPercent(flVec);
        m_br.driveDirectionPercent(brVec);
        m_bl.driveDirectionPercent(blVec);
    }
}

float SwerveDrive::strafeCalcZ(float angle, float gyroDegrees, UpdateMode mode) {
    switch (mode) {
        case UpdateMode::Percent:
            return std::clamp(m_strafeAnglePid.Calculate(gyroDegrees, angle) / 270.0, -0.5, 0.5);
        case UpdateMode::Velocity:
            return std::clamp(m_strafeAnglePid.Calculate(gyroDegrees, angle), -360.0, 360.0);
    }
}

float SwerveDrive::turnCalcZ(float angle, float gyroDegrees) {
    return std::clamp(m_turnAnglePid.Calculate(gyroDegrees, angle), -0.2, 0.2);
}