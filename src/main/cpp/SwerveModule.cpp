
#include "SwerveModule.h"
#include <math.h>

SwerveModule::SwerveModule(int driveID, int steerID, int steerEncID):
driveMotor(driveID),
steerMotor(steerID, rev::CANSparkMaxLowLevel::MotorType::kBrushless),
driveEnc(driveMotor),
steerEncNEO(steerMotor.GetEncoder()),
rawSteerEnc(steerEncID),
steerEnc(rawSteerEnc),
drivePID(0.01, 0, 0),
steerPID(0, 0, 0)
{
    // by default this selects the ingetrated sensor
    ctre::phoenix::motorcontrol::can::TalonFXConfiguration config;

    // these settings are present in the documentation example, and since they relate to safety of motor, they are probably a good idea to include
    config.supplyCurrLimit.triggerThresholdCurrent = 40; // the peak supply current, in amps
    config.supplyCurrLimit.triggerThresholdTime = 1.5; // the time at the peak supply current before the limit triggers, in sec
    config.supplyCurrLimit.currentLimit = 30; // the current to maintain if the peak supply limit is triggered

    config.velocityMeasurementPeriod = ctre::phoenix::sensors::SensorVelocityMeasPeriod::Period_25Ms;

    // FIXME: pid tune
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
    float speed = std::clamp(drivePID.Calculate(getDistance(), setpoint), -0.3, 0.3); // Calculates scaled output based off of encoder feedback. 
    driveMotor.Set(TalonFXControlMode::PercentOutput, speed); // speed is what percent power the motor is run at between [-1, 1]
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
    float ret = driveEnc.GetIntegratedSensorVelocity();
    return ret;
}

float SwerveModule::setDriveVelocity(float percent) // the onE
{
    float currentSpeed = getDriveSpeed();
    float speed = std::clamp(drivePID.Calculate(currentSpeed, percent * DRIVE_VELOCITY), -0.3, 0.3);
    driveMotor.Set(TalonFXControlMode::PercentOutput, speed); 

    return speed;
}

void SwerveModule::updateDrivePID(double pNew, double iNew, double dNew)
{   
    drivePID.SetP(pNew);
    drivePID.SetI(iNew);
    drivePID.SetD(dNew);
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

/*
    steer module to given angle 
    - return boolean (do we need to change the speed or not?)
        o specify wether or not the direction need to be reversed  
        o still move even if angle doesn't need to be adjusted
*/ 