
#include "SwerveModule.h"
#include <math.h>
#include "misc.h"

SwerveModule::SwerveModule(int driveID, int steerID, int steerEncID):
driveMotor(driveID),
steerMotor(steerID, rev::CANSparkMaxLowLevel::MotorType::kBrushless),
driveEnc(driveMotor),
steerEncNEO(steerMotor.GetEncoder()),
rawSteerEnc(steerEncID),
steerEnc(rawSteerEnc),
steerPID(0, 0, 0)
{
    // by default this selects the ingetrated sensor
    ctre::phoenix::motorcontrol::can::TalonFXConfiguration config;

    // these settings are present in the documentation example, and since they relate to safety of motor, they are probably a good idea to include
    config.supplyCurrLimit.triggerThresholdCurrent = 40; // the peak supply current, in amps
    config.supplyCurrLimit.triggerThresholdTime = 1.5; // the time at the peak supply current before the limit triggers, in sec
    config.supplyCurrLimit.currentLimit = 30; // the current to maintain if the peak supply limit is triggered

    config.velocityMeasurementPeriod = ctre::phoenix::sensors::SensorVelocityMeasPeriod::Period_25Ms;

    // this allows the motor to actually turn, pid values are set later
    config.slot0.kP = 1.0;

    driveMotor.ConfigAllSettings(config);

    steerPID.EnableContinuousInput(0.0, 360.0);
}

float SwerveModule::getDistance(void)
{
    float ret = driveEnc.GetIntegratedSensorPosition();
    return ret;
}

void SwerveModule::resetDriveEncoder(void)
{
    driveEnc.SetIntegratedSensorPosition(0);
}

void SwerveModule::resetSteerEncoder(float offset)
{
    steerEncNEO.SetPosition(offset);
}

float SwerveModule::getAngleRaw(void)
{
    float ret = steerEncNEO.GetPosition();
    return ret;
}

float SwerveModule::getAngle(void)
{
    float temp = getAngleRaw(); // get raw position
    float angle = (fmod(temp, TICKS_STEERING) / TICKS_STEERING) * 360; // convert to angle in degrees

    float adjusted = angle;
    if(angle < 0)
    {
        adjusted += 360; // bounds to 0-360
    }

    return adjusted;
}

void SwerveModule::goToPosition(float setpoint)
{
    float ticks = SwerveModule::inchesToMotorTicks(setpoint);
    driveMotor.Set(TalonFXControlMode::Position, ticks);
}

void SwerveModule::steerToAng(float setpoint) // the twO
{
    float speed = std::clamp(steerPID.Calculate(getAngle(), setpoint) / 270.0, -0.5, 0.5);                                           
    steerMotor.Set(speed);
}


void SwerveModule::setDriveSpeed(float target)
{
    driveMotor.Set(TalonFXControlMode::PercentOutput, target);
}

void SwerveModule::setSteerSpeed(float target)
{
    steerMotor.Set(target);
}

float SwerveModule::getDriveSpeed(void)
{
    return driveEnc.GetIntegratedSensorVelocity();
}

float SwerveModule::setDriveVelocity(float percent) // the onE
{
    float speed = percent * DRIVE_VELOCITY;
    float rpm = SwerveModule::wheelSpeedToRpm(speed);

    driveMotor.Set(TalonFXControlMode::Velocity, misc::rpmToTalonVel(rpm)); 

    return speed;
}

void SwerveModule::updateDrivePID(double pNew, double iNew, double dNew)
{   
    driveMotor.Config_kP(0, pNew);
    driveMotor.Config_kI(0, iNew);
    driveMotor.Config_kD(0, dNew);
}

void SwerveModule::updateSteerPID(double pNew, double iNew, double dNew)
{
    steerPID.SetP(pNew);
    steerPID.SetI(iNew);
    steerPID.SetD(dNew);
}

bool SwerveModule::adjustAngle(float targetAngle) {
    float tempCurrent = getAngle();
    float tempTarget = targetAngle;
    bool changeMade = false; 

    if(tempCurrent - tempTarget > 180) {
        tempCurrent -= 360;  
    } else if(tempCurrent - tempTarget < -180) {
        tempCurrent += 360; 
    }
    float distOfAngle = tempTarget - tempCurrent;

    if(distOfAngle > 90) {
        tempTarget -= 180; 
        changeMade = true;
    } 

    if(distOfAngle < -90)
    {
        tempTarget += 180;
        changeMade = true; 
    }

    if(tempTarget < 0) {
        tempTarget += 360;
    } else if(tempTarget > 360) {
        tempTarget -= 360;
    } 

    steerToAng(tempTarget);

    return changeMade; 
}

void SwerveModule::driveDirection(Vec2 direction) {
    if (adjustAngle(direction.angle())) {
        setDriveSpeed(-direction.magnitude());
    } else {
        setDriveSpeed(direction.magnitude());
    }
}

double SwerveModule::wheelSpeedToRpm(double speed) {
    // radians per second
    double angularVelocity = speed / (0.5 * WHEEL_DIAMETER);
    // convert to rpm
    double rpm = (60.0 * angularVelocity) / (2 * M_PI);
    // convert from wheel rpm to motor rpm
    return rpm * SWERVE_DRIVE_MOTOR_GEAR_RATIO;
}

double SwerveModule::inchesToMotorTicks(double inches) {
    // angular position in radians
    double angularPosition = inches / (0.5 * WHEEL_DIAMETER);
    // convert to encoder ticks
    double ticks = (FALCON_UNITS_PER_REV * angularPosition) / (2 * M_PI);
    // scale by gear ratio
    return ticks * SWERVE_DRIVE_MOTOR_GEAR_RATIO;
}

/*
    steer module to given angle 
    - return boolean (do we need to change the speed or not?)
        o specify wether or not the direction need to be reversed  
        o still move even if angle doesn't need to be adjusted
*/ 