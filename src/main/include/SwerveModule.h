#pragma once

#include <rev/CANSparkMax.h>

#include <frc/AnalogEncoder.h>
#include <frc/AnalogInput.h>
#include <frc/controller/PIDController.h>
#include "Prefs.h"
#include <math.h>
#include <frc/smartdashboard/SmartDashboard.h>

class SwerveModule {

    public:
        SwerveModule(int driveID, int steerID, int steerEncID);

        float getDistance(void); // Returns position of the distance encoder
        void resetDriveEncoder(void); 

        float getAngleRaw(void); // position of steering encoder
        float getAngle(void); // scaled angle between 0 and 360

        void goToPosition(float setpoint); // Position PID control, moves to specified position
        void steerToAng(float setpoint); // Angle PID control

        void setDriveSpeed(float target); // update current
        
        void setSteerSpeed(float target);
        float getDriveSpeed(void);

        float setDriveVelocity(float percent); // Drive Velocity Loop - WIP

        void updateDrivePID(double pNew, double iNew, double dNew);
        void updateSteerPID(double pNew, double iNew, double dNew);

        bool adjustAngle(float targetAngle);

    private: 

        rev::CANSparkMax a_driveMotor;
        rev::CANSparkMax a_steerMotor;

        rev::CANEncoder a_driveEnc; // Built-in NEO Encoders
        rev::CANEncoder a_steerEncNEO; 

        frc::AnalogInput a_rawSteerEnc;
        frc::AnalogEncoder a_steerEnc; // placeholder, may change wiring

        frc2::PIDController a_drivePID;
        frc2::PIDController a_steerPID;

}; 

