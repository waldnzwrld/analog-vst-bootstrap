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
                      float re,
                      float vceSat)
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
    , vceSat(vceSat)
    , alpha(beta / (beta + 1.0f))
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

void Transistor::setSaturationVoltage(float newVceSat)
{
    vceSat = newVceSat;
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
    
    // Calculate collector current in active region
    float ic = beta * ib * (1.0f + vce / va);
    
    // Check for saturation
    if (isInSaturation(vce, ib, ic)) {
        // In saturation, collector current is limited by the circuit
        // We use a simplified model that smoothly transitions to saturation
        float vceSat = calculateSaturationVoltage(ib, ic);
        float saturationFactor = std::max(0.0f, 1.0f - (vce - vceSat) / (0.1f * vceSat));
        ic *= saturationFactor;
    }
    
    // Store currents
    lastIb = ib;
    lastIc = ic;
    lastIe = ic + ib;
    
    return ic;
}

float Transistor::calculateSaturationVoltage(float ib, float ic)
{
    // Calculate saturation voltage based on current levels and temperature
    float baseVceSat = vceSat * (1.0f + (temperature - 293.15f) / 100.0f);  // Temperature compensation
    
    // Adjust saturation voltage based on current levels
    float currentRatio = std::abs(ic / (beta * ib));
    if (currentRatio > 0.9f) {
        baseVceSat *= (1.0f + (currentRatio - 0.9f) * 2.0f);
    }
    
    return baseVceSat;
}

bool Transistor::isInSaturation(float vce, float ib, float ic)
{
    // Check if the transistor is in saturation
    // For NPN: Vce < Vce(sat)
    // For PNP: Vce > -Vce(sat)
    float sign = (type == Type::NPN) ? 1.0f : -1.0f;
    float vceSat = calculateSaturationVoltage(ib, ic);
    
    return (sign * vce) < (sign * vceSat);
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