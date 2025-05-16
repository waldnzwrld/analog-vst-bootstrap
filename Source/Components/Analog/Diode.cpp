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
    // Solve for voltage using iterative method
    // This is a simplified version - in practice, you might want to use
    // a more sophisticated numerical method
    double vt = calculateThermalVoltage();
    double v = 0.0;
    const int maxIterations = 10;
    
    for (int i = 0; i < maxIterations; ++i) {
        double iCalc = calculateTotalCurrent(v);
        double error = iCalc - current;
        if (std::abs(error) < 1e-9) break;
        
        // Simple Newton-Raphson step
        double di_dv = (is / (n * vt)) * std::exp(v / (n * vt));
        v -= error / di_dv;
    }
    
    return v;
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
    // Account for series resistance
    double vd = voltage - rs * calculateForwardCurrent(voltage);
    
    if (vd >= 0) {
        return calculateForwardCurrent(vd);
    } else {
        return calculateReverseCurrent(vd);
    }
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 