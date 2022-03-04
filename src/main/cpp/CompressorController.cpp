#include "CompressorController.h"

CompressorController::CompressorController():
a_Compressor(frc::PneumaticsModuleType::REVPH) 
{
    
}

void CompressorController::update(){
    if (a_Compressor.GetPressureSwitchValue() == false) {
        a_Compressor.Disable();
    } else {
        a_Compressor.EnableDigital();
    }
}