#pragma once

#include <frc/AnalogInput.h>

class AbsoluteEncoder {
    public:
        AbsoluteEncoder(int port, double minVolts, double maxVolts);
        // offset is in degrees
        AbsoluteEncoder(int port, double minVolts, double maxVolts, double offset);

        double getAngle() const;
        double getRotations() const;

    private:
        frc::AnalogInput m_encoder;

        // starting voltage value where encoder is at 0 degrees
        double m_minVolts;
        // how many volts until the encoder will reach 360 degrees, the range of the voltage, not max
        double m_voltRange;

        // offset angle in rotations (0 - 1)
        double m_offset;
};