#include "BeamBreak.h"

BeamBreak::BeamBreak(int port):
a_beambreak(port)
{

}

bool BeamBreak::isBeamBroken() 
{
    if(a_beambreak.Get() == 1) {
        return true;
    } else {
        return false; 
    }

}