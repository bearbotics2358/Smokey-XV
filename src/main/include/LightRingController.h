#pragma once

#include <frc/I2C.h>

class LightRingController {
    public:
        LightRingController(int controllerAddress);
        bool SetColor(int device, int r, int g, int b);
        void SetAllColor(int r, int g, int b);
        bool SetFlash(int device, bool set);

    private:
        frc::I2C a_I2C;
};
