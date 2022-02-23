#pragma once

#include "Prefs.h"
#include <frc/DigitalInput.h>

class BeamBreak {
    public:
        BeamBreak(int port);
        bool isBeamBroken();

    private:
        frc::DigitalInput a_beambreak;
};