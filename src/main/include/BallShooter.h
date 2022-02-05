#pragma once

#include <rev/CANSparkMax.h>
#include <ctre/Phoenix.h>

class BallShooter 
{

    public:

        BallShooter(int idLeft, int idRight); 

        // sets speed in rpm
        void setSpeed(double rpm);
        // gets speed in rpms
        double getSpeed();
    
    private:

        TalonFX a_shooterLeft;
        TalonFX a_shooterRight;

        double alpha { 0.9 };
        double previousInput { 0.0 };

};		

