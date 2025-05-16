#include "Transistor.h"

namespace ArchitextureStudiosAnalogCore {
namespace Analog {
Transistor::Transistor(Type type)
    : type(type)
    , beta(100.0f)    // Default current gain
    , vt(0.026f)      // Default thermal voltage (26mV at room temperature)
    , is(1e-12f)      // Default saturation current
    , va(100.0f)      // Default Early voltage
{
}

void Transistor::setBeta(float newBeta)
{
    beta = newBeta;
}

void Transistor::setVt(float newVt)
{
    vt = newVt;
}

void Transistor::setIs(float newIs)
{
    is = newIs;
}

void Transistor::setEarlyVoltage(float newVa)
{
    va = newVa;
}

float Transistor::calculateCollectorCurrent(float vbe, float vce)
{
    // Ebers-Moll model implementation
    float sign = (type == Type::NPN) ? 1.0f : -1.0f;
    
    // Base-emitter current
    float ib = is * (std::exp(sign * vbe / vt) - 1.0f);
    
    // Collector current with Early effect
    float ic = beta * ib * (1.0f + vce / va);
    
    return ic;
}

float Transistor::calculateEmitterCurrent(float vbe, float vce)
{
    // Emitter current is collector current plus base current
    float ic = calculateCollectorCurrent(vbe, vce);
    float ib = ic / beta;  // Base current is collector current divided by beta
    return ic + ib;  // Emitter current is sum of collector and base currents
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore