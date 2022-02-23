#include "AbsoluteEncoder.h"

#include <math.h>

AbsoluteEncoder::AbsoluteEncoder(int port):
m_encoder(port),
m_offset(0) {}

AbsoluteEncoder::AbsoluteEncoder(int port, double offset):
m_encoder(port),
m_offset(offset / (2 * M_PI)) {}

double AbsoluteEncoder::getAngle() const {
    return 2 * M_PI * getRotations();
}

double AbsoluteEncoder::getRotations() const {
    // the encoder outputs volts in a range of 0 to 5, scaling linearly with how rotated it is
    double volts = m_encoder.GetVoltage();
    // convert volts to rotations
    return (volts / 5) - m_offset;
}