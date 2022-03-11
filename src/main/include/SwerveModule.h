
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

        // Returns position of the distance encoder in inches
        float getDistance();
        // sets the drive encoder to 0 ticks
        void resetDriveEncoder();

        // recalibrates the relative steering encoder using the absolute steering encoder
        void resetSteerEncoder();

        // scaled angle between 0 and 360
        float getAngle();
        // TEMP
        // TODO: remove
        float getAbsAngleDegrees();

        void goToPosition(float inches); // Position PID control, moves drive wheel to specified position
        void steerToAng(float degrees); // Angle PID control

        // sets drive speed in percent
        void setDrivePercent(float percent);
        // sets steer speed in percent
        void setSteerPercent(float percent);

        // set wheel speed in inches per second
        // TODO: this is a stupid unit, use meters / second
        float setDriveSpeed(float speed);

        // sets drive and steer p, i, and d values for pid
        void setDrivePID(double pNew, double iNew, double dNew);
        void setSteerPID(double pNew, double iNew, double dNew);

        // steers to the given targetAngle by taking the shortest possible path of rotation
        // this meanss the whell may end up facing backwards
        // if that is the case, this returns true to indicate that the wheel speed should be opposite of what it would normally be
        bool adjustAngle(float targetAngle);

        // drives in the direction of the vector
        // the magnitude of the vector is passed into setDrivePercent
        void driveDirection(Vec2 direction);

    private:
        // speed is inches per second
        static double wheelSpeedToRpm(double speed);
        static double inchesToMotorTicks(double inches);
        static double motorTicksToInches(double motorTicks);

        // get angle from relative encoder in degrees, does not take into consideration currently set zero point
        double getRelativeAngle();

        // how many degrees away from the actual zero degrees
        // that the relative encoder's zero point is
        double encZeroPoint { 0.0 };

        TalonFX driveMotor;
        rev::CANSparkMax steerMotor;

        TalonFXSensorCollection driveEnc; // built in TalonFX sensors
        rev::SparkMaxRelativeEncoder steerEncNEO;
        AbsoluteEncoder absSteerEnc;

        frc2::PIDController steerPID;
};