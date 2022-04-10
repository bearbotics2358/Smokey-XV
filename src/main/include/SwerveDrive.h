#pragma once

#include <frc/controller/PIDController.h>

#include "JrimmyGyro.h"
#include "Prefs.h"
#include "SwerveModule.h"
#include "math/LinAlg.h"

class SwerveDrive // Class to handle the kinematics of Swerve Drive
{
    public:
        enum class UpdateMode {
            // direction is in meters per second and rotationSpeed is in degrees per second
            Velocity,
            // direction and rotationSpeed is in percent
            Percent,
        };

        SwerveDrive(SwerveModule& flModule, SwerveModule& frModule, SwerveModule& blModule, SwerveModule& brModule, JrimmyGyro& gyro);

        // TODO: change the signs of x, because the positive is left thing is wierd
        // TODO: use meters/second vector for strafeUpdate and swerveUpdate, instead of x and y going from 0 to 1
        // for strafe drive update and swerve drive update, +y is forward, -y is backward, x is left, and -x is right
        // strafe drive is like swerve drive update, except it maintains a constant turn angle
        void strafeUpdate(Vec2 direction, bool fieldOriented, UpdateMode mode = UpdateMode::Velocity);
        void swerveUpdate(Vec2 direction, float rotationSpeed, bool fieldOriented, UpdateMode mode = UpdateMode::Velocity);
        /*
            x = x asix on joystick
            y = y axis on joystick
            z = z axis on joystick (rotation)
            gyroDegrees = sensor
            fieldOriented = if true, translation movement is relative to the field
            if false, translational movement is relative to the front of the robot,
            and it is affected by the robot's current turn angle
        */

        // stops the robot from moving, just coasts
        void stop();

        // configures the swerve drive to brake when stopped
        void brakeOnStop();

        // configures the swerve drive to coast when stopped
        void coastOnStop();

        // sets the hold angle used by strafe drive update
        void setHoldAngle(float degrees);

        // unsets the hold angle, so the next call to strafeUpdate will set the hold angle to the reading from the gyro
        void unsetHoldAngle();

        // resets steering and driving encoders
        void resetDrive();

        // dist in meters and angle 0-360
        // FIXME
        void driveDistance(float distMeters, float directionDegrees);

        /** Drives at a given percent speed in a given direction.
         *  @param percent percent output of drive motors, from -1 to 1.
         *  @param directionDegrees direction to drive in degress from 0 to 360.
         */
        // FIXME
        void driveDirection(float percent, float directionDegrees);

        // returns the average of the total distance of the drive encoders in all 4 modules in meters
        // FIXME
        float getAvgDistance();

        // angle is in degrees
        // FIXME
        void turnToAngle(float angle);

        // drives at a given speed (units uknown), in a given direction in degrees, for a given distance in meters
        // FIXME
        void goToTheDon(float speed, float direction, float distance, bool fieldOriented);

        // goes to the specified position in meters and the specified angle in degrees at the specified percent speed
        // returns true when it has reached the position and angle
        bool goToPosition(Vec2 position, float degrees, float maxSpeed);

        // updates the current position of the robot based on the change in the wheel positions
        void updatePosition();

        // gets the current position in meters
        Vec2 getPosition() const;

        // sets the current position in meters to the passed in value
        void setPosition(Vec2 position);

    private:
        // called by both strafeUpdate and swerveUpdate
        // does the bulk of the swerve drive work
        // direction and rotationSpeed units are specified by update mode
        void swerveUpdateInner(Vec2 direction, float rotationSpeed, float gyroDegrees, bool fieldOriented, UpdateMode mode);

        // uses the strafe pid to calulate the required z drive to get to the specified angle
        // TODO: calibrate for velocity mode
        float strafeCalcZ(float angle, float gyroDegrees, UpdateMode mode);

        // uses the turn pid to calculate the required z drive
        float turnCalcZ(float angle, float gyroDegrees);

        SwerveModule& m_fl;
        SwerveModule& m_fr;
        SwerveModule& m_bl;
        SwerveModule& m_br;
        JrimmyGyro& m_gyro;

        // pid when using turn to angle
        frc2::PIDController m_turnAnglePid;

        // pid when using strafeUpdate
        frc2::PIDController m_strafeAnglePid;

        // if we're in strafe drive mode
        bool m_strafe;
        // angle to hold in strafe drive mode
        float m_holdAngle;

        // current position of the robot
        Vec2 m_position { 0.0, 0.0 };
        // last position of each drive wheel in meters
        float m_flLastPos { 0.0 };
        float m_frLastPos { 0.0 };
        float m_blLastPos { 0.0 };
        float m_brLastPos { 0.0 };

        // length and width of the drive base in meters
        // it is 29.75 inches
        // TODO: measure it more precisely
        constexpr static float DRIVE_LENGTH = 0.756;
        constexpr static float DRIVE_WIDTH = 0.756;

        // for goToPosition, when the distance to the target position is within this amount, say that we are done (assuming angle is also close enough)
        constexpr static float GO_TO_DIST_DONE = 0.2;
        // for goToPosition, when the angle difference from the target angle is within this amount, say that we are done (assuming distance is also close enough)
        constexpr static float GO_TO_ANGLE_DONE = 5.0;
};