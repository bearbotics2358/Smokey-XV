#pragma once

#include <frc/AnalogInput.h>

class AbsoluteEncoder {
    public:
        AbsoluteEncoder(int port);
        // offset is in radians
        AbsoluteEncoder(int port, double offset);

        double getAngle() const;
    private:
        frc::AnalogInput m_encoder;
        // offset angle in radians
        double m_offset;
};