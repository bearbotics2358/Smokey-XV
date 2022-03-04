#pragma once

#include <frc/Compressor.h>

class CompressorController {

    public:
        CompressorController();
        void update();

    private:
        frc::Compressor a_Compressor;

};
