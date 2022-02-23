#include "BeamBreak.h"

BeamBreak::BeamBreak(int port):
a_beambreak(port) {
}

bool BeamBreak::isBeamBroken() {
    return a_beambreak.Get() == 1;
}