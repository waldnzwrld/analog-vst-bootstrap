#include "Potentiometer.h"

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

Potentiometer::Potentiometer(double totalResistance, double initialPosition)
    : Resistor(totalResistance)
    , position(std::clamp(initialPosition, MIN_POSITION, MAX_POSITION))
{
}

void Potentiometer::setPosition(double newPosition)
{
    position = std::clamp(newPosition, MIN_POSITION, MAX_POSITION);
}

double Potentiometer::getResistance1() const
{
    // Resistance between terminal 1 and wiper
    return getResistance() * position;
}

double Potentiometer::getResistance2() const
{
    // Resistance between wiper and terminal 2
    return getResistance() * (1.0 - position);
}

double Potentiometer::process(double voltage1, double voltage2)
{
    // Calculate voltage at wiper using voltage divider equation
    double totalResistance = getResistance();
    double r1 = getResistance1();
    double r2 = getResistance2();
    
    // V_wiper = V1 + (V2 - V1) * (R1 / (R1 + R2))
    return voltage1 + (voltage2 - voltage1) * (r1 / totalResistance);
}

void Potentiometer::reset()
{
    Resistor::reset();
    // No additional state to reset for now
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 