#pragma once

#include <frc/controller/PIDController.h>

#include "Prefs.h"
#include "SwerveModule.h"
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
        SwerveDrive(SwerveModule *FL_Ptr, SwerveModule *FR_Ptr, SwerveModule *BL_Ptr, SwerveModule *BR_Ptr);
        float lockZ(float gyro);

        void crabDriveUpdate(float xIn, float yIn, float gyroIn); // clutzy from club penguin



        void swerveUpdate(float xIn, float yIn, float zIn, float gyroIn, bool fieldOriented); // Updates Swerve Modules for swerve drive
        /*
            xIn = x asix on joystick
            yIn = y axis on joystick
            zIn =  z axis on joystick
            gyroIn = sensor
            fieldOriented = are you looking at the field head on or not?
        */


        void driveDistance(float dist, float direction); // dist in inches and angle 0-360
        void resetDrive();
        float getAvgDistance(void); // baka-47

        void turnToAngle(float gyro, float angle);

        void makeShiftTurn(float speed);

        void GoToTheDon(float speed, float direction, float distance, float gyro);

    private:
        SwerveModule *FL_Module;
        SwerveModule *FR_Module;
        SwerveModule *BL_Module;
        SwerveModule *BR_Module;

        frc2::PIDController anglePID;
        frc2::PIDController jenkinsTheCrabPID; // blame kordt

        float holdAngle;
        bool crab; // if we're in crab drive mode

        const float DRIVE_LENGTH = 29.75;
        const float DRIVE_WIDTH = 29.75;

        const float PI = M_PI;

        /* frc::AnalogInput FL_Input;
        frc::AnalogInput FR_Input;
        frc::AnalogInput BL_Input;
        frc::AnalogInput BR_Input;

        frc::AnalogEncoder FL_Encoder;
        frc::AnalogEncoder FR_Encoder;
        frc::AnalogEncoder BL_Encoder;
        frc::AnalogEncoder BR_Encoder; */
};
#endif