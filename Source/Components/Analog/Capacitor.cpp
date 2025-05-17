#include "Capacitor.h"

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

Capacitor::Capacitor(double capValue) :
    capacitance(std::clamp(capValue, MIN_CAPACITANCE, MAX_CAPACITANCE)),
    voltage(0.0),
    lastVoltage(0.0),
    sampleRate(44100.0),
    dt(1.0/44100.0)
{
}

void Capacitor::setCapacitance(double capValue) {
    capacitance = std::clamp(capValue, MIN_CAPACITANCE, MAX_CAPACITANCE);
}

void Capacitor::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
    dt = 1.0 / sampleRate;
}

double Capacitor::process(double inputVoltage) {
    // Calculate current using I = C * dV/dt
    // Using backward Euler method for numerical integration
    double current = capacitance * (inputVoltage - lastVoltage) / dt;
    
    // Update voltage across capacitor
    // V = (1/C) * ∫I dt
    voltage = lastVoltage + (current * dt / capacitance);
    
    // Store current voltage for next iteration
    lastVoltage = voltage;
    
    return voltage;
}

void Capacitor::reset() {
    voltage = 0.0;
    lastVoltage = 0.0;
}

double Capacitor::getVoltage() const {
    return voltage;
}

double Capacitor::getCurrent(double inputVoltage) const {
    return capacitance * (inputVoltage - lastVoltage) / dt;
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 