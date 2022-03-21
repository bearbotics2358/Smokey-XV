#pragma once

#include <frc/controller/PIDController.h>

#include "Prefs.h"
#include "SwerveModule.h"
#include "JrimmyGyro.h"
#include "math/LinAlg.h"

#ifdef NEW_SWERVE
struct SwerveTransform {
        SwerveTransform(Vec2 direction, float rotSpeed);

        static SwerveTransform translate(Vec2 direction, float gyroAngle, bool fieldOriented = true);
        static SwerveTransform translateRotate(Vec2 direction, float rotSpeed, float gyroAngle, bool fieldOriented);

        // direction for robot to move in
        Vec2 direction;

        // speed of rotation in radians
        // if this is 0, the robot will try and hold its current angle
        float rotSpeed;
};

class SwerveDrive {
    public:
        SwerveDrive(SwerveModule& flModule, SwerveModule& frModule, SwerveModule& blModule, SwerveModule& brModule);
        ~SwerveDrive();

        void update(const SwerveTransform& transform);

        float getAvgDistance() const;

    private:
        SwerveModule& m_fl;
        SwerveModule& m_fr;
        SwerveModule& m_bl;
        SwerveModule& m_br;

        // angle to hold if rotSpeed is 0
        float m_holdAngle { 0 };

        // pid controller to use when holding angle
        frc2::PIDController m_anglePid;

        static constexpr float DRIVE_LENGTH { 29.75 };
        static constexpr float DRIVE_WIDTH { 29.75 };
};

#else

class SwerveDrive // Class to handle the kinematics of Swerve Drive
{
    public:
        SwerveDrive(SwerveModule& flModule, SwerveModule& frModule, SwerveModule& blModule, SwerveModule& brModule, JrimmyGyro& gyro);

        // crab drive is like swerve drive update, except it maintains a constant turn angle
        void crabDriveUpdate(float x, float y, bool fieldOriented = true);
        void swerveUpdate(float x, float y, float z, bool fieldOriented);
        /*
            x = x asix on joystick
            y = y axis on joystick
            z = z axis on joystick (rotation)
            gyroDegrees = sensor
            fieldOriented = if true, translation movement is relative to the field
            if false, translational movement is relative to the front of the robot,
            and it is affected by the robot's current turn angle
        */

       // stops the robot from moving
       void stop();

       // sets the hold angle used by crab drive update
       void setHoldAngle(float degrees);

        // unsets the hold angle, so the next call to crabDriveUpdate will set the hold angle to the reading from the gyro
        void unsetHoldAngle();

        // resets steering and driving encoders
        void resetDrive();

        // dist in meters and angle 0-360
        void driveDistance(float distMeters, float directionDegrees);

        /** Drives at a given percent speed in a given direction.
         *  @param percent percent output of drive motors, from -1 to 1.
         *  @param directionDegrees direction to drive in degress from 0 to 360.
         */
        void driveDirection(float percent, float directionDegrees);

        // returns the average of the total distance of the drive encoders in all 4 modules in meters
        float getAvgDistance();

        // angle is in degrees
        void turnToAngle(float angle);

        // drives at a given speed (units uknown), in a given direction in degrees, for a given distance in meters
        void goToTheDon(float speed, float direction, float distance, bool fieldOriented = true);

    private:
        // called by both crabDriveUpdate and swerveUpdata
        // does the bulk of the swerve drive work
        // x and y are translation, z is rotation
        void swerveUpdateInner(float x, float y, float z, float gyroDegrees, bool fieldOriented);

        SwerveModule& flModule;
        SwerveModule& frModule;
        SwerveModule& blModule;
        SwerveModule& brModule;
        JrimmyGyro& a_gyro;

        // pid when using turn to angle
        frc2::PIDController turnAnglePid;

        // pid when using crabDriveUpdate
        frc2::PIDController crabAnglePid;

        // if we're in crab drive mode
        bool crab;
        // angle to hold in crab drive mode
        float holdAngle;

        constexpr static float DRIVE_LENGTH = 29.75;
        constexpr static float DRIVE_WIDTH = 29.75;
};
#endif