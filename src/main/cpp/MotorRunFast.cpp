#include "MotorRunFast.h"
#include "Prefs.h"

    MotorRunFast::MotorRunFast() :
    a_speed(SPEED_COLLECTOR_ID)
    {

    }

    void MotorRunFast::MotorGoBurrr() 
    {

        a_speed.Set(1000); 

    }
