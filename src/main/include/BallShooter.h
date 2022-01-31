#pragma once

#include "Prefs.h"

#include <rev/CANSparkMax.h>
#include <ctre/Phoenix.h>

class BallShooter 
{

    public:

        BallShooter(int id); 

        void SetSpeed(double speed); 
    
    private: 
        
        TalonFX a_Shooter; 

};		

