
#pragma once

#include "AbsoluteEncoder.h"
#include "Prefs.h"
#include "math/LinAlg.h"
#include <ctre/Phoenix.h>
#include <frc/AnalogEncoder.h>
#include <frc/AnalogInput.h>
#include <frc/controller/PIDController.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <math.h>
#include <rev/CANSparkMax.h>

class SwerveModule // Handles steering and driving of each Swerve Module
{
    public:
        SwerveModule(int driveID, int steerID, AbsoluteEncoder&& absEncoder); // CAN IDs, analog port for steer encoder

        float getDistance(); // Returns position of the distance encoder
        void resetDriveEncoder();

        void resetSteerEncoder();

        float getAngle(); // scaled angle between 0 and 360
        // TEMP
        // TODO: remove
        float getAbsAngleDegrees();

        void goToPosition(float setpoint); // Position PID control, moves to specified position
        void steerToAng(float setpoint); // Angle PID control

        void setDriveSpeed(float target); // update current

        void setSteerSpeed(float target);
        float getDriveSpeed();

        float setDriveVelocity(float percent); // Drive Velocity Loop - WIP

        void updateDrivePID(double pNew, double iNew, double dNew);
        void updateSteerPID(double pNew, double iNew, double dNew);

        bool adjustAngle(float targetAngle);

        // drives in the direction of the vector
        // the magnitude of the vector is passed into setDriveSpeed
        void driveDirection(Vec2 direction);

    private:
        static double wheelSpeedToRpm(double speed);
        static double inchesToMotorTicks(double inches);

        // get angle from relative encoder in degrees
        double getRelativeAngle();

        // how many degrees away from the actual zero degrees
        // that the relative encoder's zero point is
        double encZeroPoint;

        TalonFX driveMotor;
        rev::CANSparkMax steerMotor;

        TalonFXSensorCollection driveEnc; // built in TalonFX sensors
        rev::SparkMaxRelativeEncoder steerEncNEO;
        AbsoluteEncoder absSteerEnc;

        frc2::PIDController steerPID;
};