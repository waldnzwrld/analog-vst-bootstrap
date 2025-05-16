#include "Resistor.h"

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

Resistor::Resistor(double resistance) :
    resistance(std::clamp(resistance, MIN_RESISTANCE, MAX_RESISTANCE)),
    temperature(DEFAULT_TEMPERATURE)
{
}

void Resistor::setResistance(double newResistance) {
    resistance = std::clamp(newResistance, MIN_RESISTANCE, MAX_RESISTANCE);
}

double Resistor::process(double voltage) {
    // Calculate current using Ohm's Law: I = V/R
    return getCurrent(voltage);
}

double Resistor::getCurrent(double voltage) const {
    // I = V/R
    return voltage / resistance;
}

double Resistor::getVoltage(double current) const {
    // V = I*R
    return current * resistance;
}

double Resistor::getPower(double voltage) const {
    // P = V²/R
    return (voltage * voltage) / resistance;
}

void Resistor::reset() {
    // Currently no state to reset, but kept for future expansion
    // (e.g., if we add thermal modeling)
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 