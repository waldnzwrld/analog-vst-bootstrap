#include "Potentiometer.h"
#include <algorithm>

namespace ArchitextureStudiosAnalogCore::Analog {

Potentiometer::Potentiometer(double resistance, double initialPosition, TaperType taper)
    : totalResistance(resistance)
    , position(std::min(std::max(initialPosition, MIN_POSITION), MAX_POSITION))
    , taperType(taper)
{
}

void Potentiometer::setPosition(double newPosition)
{
    position = std::min(std::max(newPosition, MIN_POSITION), MAX_POSITION);
}

double Potentiometer::getResistance1() const
{
    return totalResistance * getEffectivePosition();
}

double Potentiometer::getResistance2() const
{
    return totalResistance * (1.0 - getEffectivePosition());
}

double Potentiometer::process(double voltage1, double voltage2)
{
    // Calculate voltage at wiper using voltage divider formula with effective position
    return voltage1 + (voltage2 - voltage1) * getEffectivePosition();
}

double Potentiometer::getEffectivePosition() const
{
    switch (taperType) {
        case TaperType::Logarithmic:
            // Logarithmic taper: y = a^x where a is chosen to give a good curve
            // Using a = 10 gives a good audio taper curve
            return (std::pow(10.0, position) - 1.0) / 9.0;
            
        case TaperType::Linear:
        default:
            return position;
    }
}

} // namespace ArchitextureStudiosAnalogCore::Analog 