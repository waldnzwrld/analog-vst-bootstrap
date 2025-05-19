#include "Capacitor.h"
#include <algorithm>  // For std::clamp

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

void Capacitor::initializeCharacteristics() {
    switch (type) {
        case Type::Film:
            characteristics = {
                0.25,    // ESR (250mΩ typical for film caps)
                2.5e-9,  // ESL (2.5nH)
                0.002,   // DA (0.2% typical for film)
                -150e-6, // Temp coef cap (-150ppm/°C)
                0.002,   // Temp coef ESR (0.2%/°C)
                400.0,   // Max voltage (400V typical)
                233.15,  // Min temp (-40°C)
                373.15   // Max temp (100°C)
            };
            break;
            
        case Type::Ceramic:
            characteristics = {
                0.015,   // ESR (15mΩ typical for ceramic)
                0.3e-9,  // ESL (0.3nH)
                0.02,    // DA (2% typical for ceramic)
                -1500e-6,// Temp coef cap (-1500ppm/°C for X7R)
                0.001,   // Temp coef ESR (0.1%/°C)
                50.0,    // Max voltage (50V typical)
                233.15,  // Min temp (-40°C)
                398.15   // Max temp (125°C)
            };
            break;
            
        case Type::Electrolytic:
            characteristics = {
                0.1,     // ESR (100mΩ typical for electrolytic)
                5.0e-9,  // ESL (5nH)
                0.05,    // DA (5% typical for electrolytic)
                -1000e-6,// Temp coef cap (-1000ppm/°C)
                0.005,   // Temp coef ESR (0.5%/°C)
                16.0,    // Max voltage (16V typical)
                233.15,  // Min temp (-40°C)
                358.15   // Max temp (85°C)
            };
            break;
            
        case Type::Mica:
            characteristics = {
                0.05,    // ESR (50mΩ typical for mica)
                1.0e-9,  // ESL (1nH)
                0.001,   // DA (0.1% typical for mica)
                50e-6,   // Temp coef cap (50ppm/°C)
                0.001,   // Temp coef ESR (0.1%/°C)
                500.0,   // Max voltage (500V typical)
                233.15,  // Min temp (-40°C)
                398.15   // Max temp (125°C)
            };
            break;
            
        case Type::Paper:
            characteristics = {
                0.5,     // ESR (500mΩ typical for paper)
                10.0e-9, // ESL (10nH)
                0.01,    // DA (1% typical for paper)
                -200e-6, // Temp coef cap (-200ppm/°C)
                0.003,   // Temp coef ESR (0.3%/°C)
                600.0,   // Max voltage (600V typical)
                233.15,  // Min temp (-40°C)
                373.15   // Max temp (100°C)
            };
            break;
            
        case Type::Custom:
            // Keep existing values
            break;
    }
    
    // Initialize ESR and ESL from characteristics
    esr = characteristics.defaultESR;
    esl = characteristics.defaultESL;
}

Capacitor::Capacitor(Type type, double capValue, double temp) :
    type(type),
    capacitance(std::clamp(capValue, MIN_CAPACITANCE, MAX_CAPACITANCE)),
    temperature(temp),
    voltage(0.0),
    lastVoltage(0.0),
    lastCurrent(0.0),
    sampleRate(44100.0),
    dt(1.0/44100.0)
{
    // Initialize characteristics based on type
    initializeCharacteristics();
    
    // Initialize DA time constants (in seconds) for different mechanisms
    daTimeConstants = {
        0.001,   // 1ms
        0.01,    // 10ms
        0.1,     // 100ms
        1.0,     // 1s
        10.0,    // 10s
        100.0,   // 100s
        1000.0,  // 1000s
        10000.0  // 10000s
    };
    
    // Initialize DA voltages
    daVoltages.fill(0.0);
}

void Capacitor::setType(Type newType) {
    type = newType;
    initializeCharacteristics();
}

void Capacitor::setCapacitance(double capValue) {
    capacitance = std::clamp(capValue, MIN_CAPACITANCE, MAX_CAPACITANCE);
}

void Capacitor::setESR(double newESR) {
    if (type == Type::Custom) {
        esr = newESR;
    }
}

void Capacitor::setESL(double newESL) {
    if (type == Type::Custom) {
        esl = newESL;
    }
}

void Capacitor::setTemperature(double temp) {
    temperature = std::clamp(temp, characteristics.minTemp, characteristics.maxTemp);
}

double Capacitor::calculateTemperatureAdjustedCapacitance() const {
    // Calculate temperature difference from reference (20°C = 293.15K)
    double tempDiff = temperature - 293.15;
    // Apply temperature coefficient
    return capacitance * (1.0 + (characteristics.tempCoefCap * tempDiff));
}

double Capacitor::calculateTemperatureAdjustedESR() const {
    // Calculate temperature difference from reference (20°C = 293.15K)
    double tempDiff = temperature - 293.15;
    // Apply temperature coefficient
    return esr * (1.0 + (characteristics.tempCoefESR * tempDiff));
}

double Capacitor::calculateDielectricAbsorptionVoltage() const {
    double daVoltage = 0.0;
    
    // Sum up contributions from all time constants
    for (size_t i = 0; i < DA_HISTORY_SIZE; ++i) {
        // Calculate exponential decay for each time constant
        double decay = std::exp(-dt / daTimeConstants[i]);
        // Add weighted contribution
        daVoltage += daVoltages[i] * decay;
    }
    
    return daVoltage * characteristics.dielectricAbsorption;
}

void Capacitor::updateDAVoltages(double newVoltage) {
    // Update each DA voltage based on voltage change
    for (size_t i = 0; i < DA_HISTORY_SIZE; ++i) {
        // Calculate exponential decay
        double decay = std::exp(-dt / daTimeConstants[i]);
        // Update with new voltage and decay
        daVoltages[i] = (newVoltage - lastVoltage) * (1.0 - decay) + daVoltages[i] * decay;
    }
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
    
    // Calculate dielectric absorption voltage
    double daVoltage = calculateDielectricAbsorptionVoltage();
    
    // Update voltage across capacitor including all effects
    voltage = lastVoltage + (current * dt / tempAdjustedCap) + eslVoltage + esrVoltage + daVoltage;
    
    // Update DA voltages for next iteration
    updateDAVoltages(voltage);
    
    // Store current values for next iteration
    lastVoltage = voltage;
    lastCurrent = current;
    
    return voltage;
}

void Capacitor::reset() {
    voltage = 0.0;
    lastVoltage = 0.0;
    lastCurrent = 0.0;
    daVoltages.fill(0.0);
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