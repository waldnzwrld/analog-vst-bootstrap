#include "Resistor.h"
#include <algorithm>
#include <cmath>
#include <random>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

Resistor::Resistor(double resistance, double tempCoeff, double powerRating,
                  double parasiticCap, double parasiticInd)
    : nominalResistance(std::clamp(resistance, MIN_RESISTANCE, MAX_RESISTANCE))
    , actualResistance(nominalResistance)
    , tempCoeff(tempCoeff)
    , powerRating(powerRating)
    , parasiticCap(parasiticCap)
    , parasiticInd(parasiticInd)
    , temperature(DEFAULT_TEMPERATURE)
    , powerDissipation(0.0)
    , lastVoltage(0.0)
    , lastCurrent(0.0)
{
    updateActualResistance();
}

void Resistor::setResistance(double newResistance) {
    nominalResistance = std::clamp(newResistance, MIN_RESISTANCE, MAX_RESISTANCE);
    updateActualResistance();
}

void Resistor::setTemperature(double temp) {
    temperature = std::clamp(temp, MIN_TEMPERATURE, MAX_TEMPERATURE);
    updateActualResistance();
}

double Resistor::process(double voltage) {
    // Calculate current using actual resistance
    double current = getCurrent(voltage);
    
    // Update power dissipation
    powerDissipation = getPower(voltage);
    
    // Update actual resistance based on new power dissipation
    updateActualResistance();
    
    // Store state
    lastVoltage = voltage;
    lastCurrent = current;
    
    return current;
}

double Resistor::getCurrent(double voltage) const {
    // I = V/R using actual resistance
    return voltage / actualResistance;
}

double Resistor::getVoltage(double current, double frequency) const {
    // Calculate frequency-dependent effects
    double freqEffect = calculateFrequencyEffect(frequency);
    
    // V = I*R with frequency effects
    return current * actualResistance * freqEffect;
}

double Resistor::getPower(double voltage) const {
    // P = V²/R
    return (voltage * voltage) / actualResistance;
}

double Resistor::getThermalNoise() const {
    // Johnson-Nyquist noise: V = sqrt(4 * k * T * R * B)
    // where k is Boltzmann's constant, T is temperature, R is resistance, B is bandwidth
    // For audio, we use a bandwidth of 20kHz
    const double bandwidth = 20000.0; // 20kHz
    return std::sqrt(4.0 * BOLTZMANN_CONSTANT * temperature * actualResistance * bandwidth);
}

double Resistor::getActualResistance() const {
    return actualResistance;
}

void Resistor::reset() {
    lastVoltage = 0.0;
    lastCurrent = 0.0;
    powerDissipation = 0.0;
    updateActualResistance();
}

void Resistor::updateActualResistance() {
    // Calculate temperature effect
    double tempEffect = calculateTemperatureEffect();
    
    // Calculate power effect
    double powerEffect = calculatePowerEffect();
    
    // Update actual resistance
    actualResistance = nominalResistance * tempEffect * powerEffect;
}

double Resistor::calculateTemperatureEffect() const {
    // R = R0 * (1 + α * (T - T0))
    // where α is temperature coefficient, T0 is reference temperature (20°C)
    return 1.0 + tempCoeff * (temperature - DEFAULT_TEMPERATURE);
}

double Resistor::calculatePowerEffect() const {
    // Resistance increases with power dissipation
    // R = R0 * (1 + P/P_rated)
    // where P is power dissipation, P_rated is power rating
    return 1.0 + (powerDissipation / powerRating);
}

double Resistor::calculateFrequencyEffect(double frequency) const {
    if (frequency <= 0.0) {
        return 1.0;  // No frequency effects at DC
    }
    
    // Calculate reactance of parasitic elements
    double omega = 2.0 * M_PI * frequency;
    double xc = 1.0 / (omega * parasiticCap);  // Capacitive reactance
    double xl = omega * parasiticInd;          // Inductive reactance
    
    // Calculate parallel impedance of parasitic elements
    double parasiticZ = (xc * xl) / std::sqrt(xc * xc + xl * xl);
    
    // Calculate total impedance including parasitic effects
    double totalZ = std::sqrt(actualResistance * actualResistance + parasiticZ * parasiticZ);
    
    // Return ratio of total impedance to actual resistance
    return totalZ / actualResistance;
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 