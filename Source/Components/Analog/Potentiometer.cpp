#include "Potentiometer.h"
#include <algorithm>

namespace ArchitextureStudiosAnalogCore::Analog {

Potentiometer::Potentiometer(double resistance, double initialPosition)
    : totalResistance(resistance)
    , position(std::min(std::max(initialPosition, MIN_POSITION), MAX_POSITION))
{
}

void Potentiometer::setPosition(double newPosition)
{
    position = std::min(std::max(newPosition, MIN_POSITION), MAX_POSITION);
}

double Potentiometer::getResistance1() const
{
    return totalResistance * position;
}

double Potentiometer::getResistance2() const
{
    return totalResistance * (1.0 - position);
}

double Potentiometer::process(double voltage1, double voltage2)
{
    // Calculate voltage at wiper using voltage divider formula
    return voltage1 + (voltage2 - voltage1) * position;
}

} // namespace ArchitextureStudiosAnalogCore::Analog 