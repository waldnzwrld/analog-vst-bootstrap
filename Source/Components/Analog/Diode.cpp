#include "Diode.h"
#include <cmath>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

Diode::Diode(double is, double n, double rs, double temp) :
    is(is),
    n(n),
    rs(rs),
    temperature(temp)
{
}

double Diode::process(double voltage) {
    return getCurrent(voltage);
}

double Diode::getCurrent(double voltage) const {
    return calculateTotalCurrent(voltage);
}

double Diode::getVoltage(double current) const {
    double vt = calculateThermalVoltage();
    
    // For forward bias (positive current)
    if (current > 0) {
        // Solve Shockley equation for voltage
        return n * vt * std::log(current / is + 1.0);
    }
    // For reverse bias (negative current)
    else {
        // More accurate reverse voltage calculation using Shockley equation
        return n * vt * std::log(1.0 + current / is);
    }
}

void Diode::setTemperature(double temp) {
    temperature = temp;
}

void Diode::setSaturationCurrent(double newIs) {
    is = newIs;
}

void Diode::setIdealityFactor(double newN) {
    n = newN;
}

void Diode::setSeriesResistance(double newRs) {
    rs = newRs;
}

void Diode::reset() {
    // No state to reset in this model
}

double Diode::calculateThermalVoltage() const {
    return (k * temperature) / q;
}

double Diode::calculateForwardCurrent(double voltage) const {
    double vt = calculateThermalVoltage();
    // Shockley equation for forward current
    return is * (std::exp(voltage / (n * vt)) - 1.0);
}

double Diode::calculateReverseCurrent(double voltage) const {
    // Simplified reverse current model
    // In practice, you might want to add breakdown effects
    return -is * (1.0 - std::exp(voltage / (n * calculateThermalVoltage())));
}

double Diode::calculateTotalCurrent(double voltage) const {
    double vt = calculateThermalVoltage();
    
    // Forward bias
    if (voltage > 0) {
        // Account for series resistance
        double vd = voltage - 0.7;  // Subtract forward voltage drop
        if (vd > 0) {
            return vd / rs;  // Current limited by series resistance
        }
        return calculateForwardCurrent(voltage);
    }
    // Reverse bias
    else if (voltage < 0) {
        return calculateReverseCurrent(voltage);
    }
    // Below threshold
    else {
        return 0.0;
    }
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 