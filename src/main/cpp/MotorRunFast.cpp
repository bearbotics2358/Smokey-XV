#include "MotorRunFast.h"
#include "Prefs.h"

    MotorRunFast::MotorRunFast() :
    a_speed(SPEED_COLLECTOR_ID, rev::CANSparkMaxLowLevel::MotorType::kBrushless)
    {

    }

    void MotorRunFast::MotorGoBurrr() 
    {

        a_speed.Set(1000); 

    }
