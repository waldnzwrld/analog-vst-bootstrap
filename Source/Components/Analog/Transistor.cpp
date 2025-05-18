#include "Transistor.h"

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

Transistor::Transistor(Type type,
                      float beta,
                      float vt,
                      float is,
                      float va,
                      float temp,
                      float cbc,
                      float rb,
                      float rc,
                      float re)
    : type(type)
    , beta(beta)
    , vt(vt)
    , is(is)
    , va(va)
    , temperature(temp)
    , cbc(cbc)
    , rb(rb)
    , rc(rc)
    , re(re)
    , lastVbe(0.0f)
    , lastVce(0.0f)
    , lastIc(0.0f)
    , lastIb(0.0f)
    , lastIe(0.0f)
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

void Transistor::setTemperature(float temp)
{
    temperature = temp;
    // Update thermal voltage based on temperature
    vt = (k * temperature) / q;
}

void Transistor::setMillerCap(float newCbc)
{
    cbc = newCbc;
}

void Transistor::setBaseResistance(float newRb)
{
    rb = newRb;
}

void Transistor::setCollectorResistance(float newRc)
{
    rc = newRc;
}

void Transistor::setEmitterResistance(float newRe)
{
    re = newRe;
}

float Transistor::calculateCollectorCurrent(float vbe, float vce)
{
    // Store last voltages
    lastVbe = vbe;
    lastVce = vce;
    
    // Ebers-Moll model implementation with temperature effects
    float sign = (type == Type::NPN) ? 1.0f : -1.0f;
    
    // Base-emitter current with temperature effects
    float ib = is * (std::exp(sign * vbe / vt) - 1.0f);
    
    // Collector current with Early effect and temperature
    float ic = beta * ib * (1.0f + vce / va);
    
    // Store currents
    lastIb = ib;
    lastIc = ic;
    lastIe = ic + ib;
    
    return ic;
}

float Transistor::calculateEmitterCurrent(float vbe, float vce)
{
    // Emitter current is collector current plus base current
    float ic = calculateCollectorCurrent(vbe, vce);
    float ib = calculateBaseCurrent(vbe, vce);
    return ic + ib;
}

float Transistor::calculateBaseCurrent(float vbe, float vce)
{
    float sign = (type == Type::NPN) ? 1.0f : -1.0f;
    return is * (std::exp(sign * vbe / vt) - 1.0f);
}

float Transistor::calculateMillerCapacitance(float vce, float gain)
{
    // Miller effect: Cm = Cbc * (1 + |Av|)
    // where Av is the voltage gain
    return cbc * (1.0f + std::abs(gain));
}

void Transistor::reset()
{
    lastVbe = 0.0f;
    lastVce = 0.0f;
    lastIc = 0.0f;
    lastIb = 0.0f;
    lastIe = 0.0f;
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore