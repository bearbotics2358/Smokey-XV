#include <ctre/Phoenix.h>

class MotorRunFast {

    public: 

        MotorRunFast(); 
        void MotorGoBurrr(void); 

    private: 

        WPI_TalonSRX a_speed;

}; 