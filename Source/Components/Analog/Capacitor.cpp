#include "Capacitor.h"

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

Capacitor::Capacitor(double capValue, double esr, double esl, double temp) :
    capacitance(std::clamp(capValue, MIN_CAPACITANCE, MAX_CAPACITANCE)),
    esr(std::clamp(esr, MIN_ESR, MAX_ESR)),
    esl(std::clamp(esl, MIN_ESL, MAX_ESL)),
    temperature(std::clamp(temp, MIN_TEMP, MAX_TEMP)),
    voltage(0.0),
    lastVoltage(0.0),
    lastCurrent(0.0),
    sampleRate(44100.0),
    dt(1.0/44100.0)
{
}

void Capacitor::setCapacitance(double capValue) {
    capacitance = std::clamp(capValue, MIN_CAPACITANCE, MAX_CAPACITANCE);
}

void Capacitor::setESR(double newESR) {
    esr = std::clamp(newESR, MIN_ESR, MAX_ESR);
}

void Capacitor::setESL(double newESL) {
    esl = std::clamp(newESL, MIN_ESL, MAX_ESL);
}

void Capacitor::setTemperature(double temp) {
    temperature = std::clamp(temp, MIN_TEMP, MAX_TEMP);
}

double Capacitor::calculateTemperatureAdjustedCapacitance() const {
    // Calculate temperature difference from reference (20°C = 293.15K)
    double tempDiff = temperature - 293.15;
    // Apply temperature coefficient
    return capacitance * (1.0 + (TEMP_COEF_CAP * tempDiff));
}

double Capacitor::calculateTemperatureAdjustedESR() const {
    // Calculate temperature difference from reference (20°C = 293.15K)
    double tempDiff = temperature - 293.15;
    // Apply temperature coefficient
    return esr * (1.0 + (TEMP_COEF_ESR * tempDiff));
}

void Capacitor::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
    dt = 1.0 / sampleRate;
}

double Capacitor::process(double inputVoltage) {
    // Get temperature-adjusted values
    double tempAdjustedCap = calculateTemperatureAdjustedCapacitance();
    double tempAdjustedESR = calculateTemperatureAdjustedESR();
    
    // Calculate current using I = C * dV/dt
    // Using backward Euler method for numerical integration
    double current = tempAdjustedCap * (inputVoltage - lastVoltage) / dt;
    
    // Add ESL effect: V = L * dI/dt
    double eslVoltage = esl * (current - lastCurrent) / dt;
    
    // Add ESR effect: V = I * R (using temperature-adjusted ESR)
    double esrVoltage = current * tempAdjustedESR;
    
    // Update voltage across capacitor including ESR and ESL effects
    voltage = lastVoltage + (current * dt / tempAdjustedCap) + eslVoltage + esrVoltage;
    
    // Store current values for next iteration
    lastVoltage = voltage;
    lastCurrent = current;
    
    return voltage;
}

void Capacitor::reset() {
    voltage = 0.0;
    lastVoltage = 0.0;
    lastCurrent = 0.0;
}

double Capacitor::getVoltage() const {
    return voltage;
}

double Capacitor::getCurrent(double inputVoltage) const {
    double tempAdjustedCap = calculateTemperatureAdjustedCapacitance();
    return tempAdjustedCap * (inputVoltage - lastVoltage) / dt;
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 