#include "Diode.h"
#include <cmath>
#include <algorithm>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

void Diode::initializeCharacteristics() {
    switch (type) {
        case Type::Silicon:
            characteristics = {
                1e-12,    // Is (1pA typical for silicon)
                1.0,      // n (ideality factor)
                0.1,      // Rs (100mΩ series resistance)
                100.0,    // Breakdown voltage (100V typical)
                1e-12,    // Junction capacitance (1pF)
                1e-9,     // Transit time (1ns)
                233.15,   // Min temp (-40°C)
                398.15,   // Max temp (125°C)
                0.15,     // Temp coef Is (15%/°C)
                -2.0      // Temp coef Vf (-2mV/°C)
            };
            break;
            
        case Type::Zener:
            characteristics = {
                1e-9,     // Is (1nA typical for zener)
                1.0,      // n (ideality factor)
                0.5,      // Rs (500mΩ series resistance)
                5.1,      // Breakdown voltage (5.1V typical)
                5e-12,    // Junction capacitance (5pF)
                1e-9,     // Transit time (1ns)
                233.15,   // Min temp (-40°C)
                398.15,   // Max temp (125°C)
                0.15,     // Temp coef Is (15%/°C)
                0.1       // Temp coef Vf (0.1mV/°C)
            };
            break;
            
        case Type::Germanium:
            characteristics = {
                1e-6,     // Is (1µA typical for germanium)
                1.0,      // n (ideality factor)
                0.2,      // Rs (200mΩ series resistance)
                50.0,     // Breakdown voltage (50V typical)
                2e-12,    // Junction capacitance (2pF)
                2e-9,     // Transit time (2ns)
                233.15,   // Min temp (-40°C)
                373.15,   // Max temp (100°C)
                0.15,     // Temp coef Is (15%/°C)
                -2.0      // Temp coef Vf (-2mV/°C)
            };
            break;
            
        case Type::LED:
            characteristics = {
                1e-10,    // Is (100pA typical for LED)
                2.0,      // n (ideality factor, higher for LEDs)
                0.5,      // Rs (500mΩ series resistance)
                5.0,      // Breakdown voltage (5V typical)
                10e-12,   // Junction capacitance (10pF)
                5e-9,     // Transit time (5ns)
                233.15,   // Min temp (-40°C)
                373.15,   // Max temp (100°C)
                0.15,     // Temp coef Is (15%/°C)
                -2.0      // Temp coef Vf (-2mV/°C)
            };
            break;
            
        case Type::Schottky:
            characteristics = {
                1e-8,     // Is (10nA typical for schottky)
                1.0,      // n (ideality factor)
                0.05,     // Rs (50mΩ series resistance)
                30.0,     // Breakdown voltage (30V typical)
                0.5e-12,  // Junction capacitance (0.5pF)
                0.1e-9,   // Transit time (0.1ns)
                233.15,   // Min temp (-40°C)
                398.15,   // Max temp (125°C)
                0.15,     // Temp coef Is (15%/°C)
                -1.5      // Temp coef Vf (-1.5mV/°C)
            };
            break;
            
        case Type::Custom:
            // Keep existing values
            break;
    }
    
    // Initialize parameters from characteristics
    is = characteristics.defaultIs;
    n = characteristics.defaultN;
    rs = characteristics.defaultRs;
    breakdown = characteristics.defaultBreakdown;
    junctionCap = characteristics.defaultJunctionCap;
    transitTime = characteristics.defaultTransitTime;
}

Diode::Diode(Type type, double temp) :
    type(type),
    temperature(temp),
    lastVoltage(0.0),
    lastCurrent(0.0)
{
    initializeCharacteristics();
}

void Diode::setType(Type newType) {
    type = newType;
    initializeCharacteristics();
}

void Diode::setTemperature(double temp) {
    temperature = std::clamp(temp, characteristics.minTemp, characteristics.maxTemp);
}

void Diode::setSaturationCurrent(double newIs) {
    if (type == Type::Custom) {
        is = newIs;
    }
}

void Diode::setIdealityFactor(double newN) {
    if (type == Type::Custom) {
        n = newN;
    }
}

void Diode::setSeriesResistance(double newRs) {
    if (type == Type::Custom) {
        rs = newRs;
    }
}

void Diode::setBreakdownVoltage(double newBreakdown) {
    if (type == Type::Custom) {
        breakdown = newBreakdown;
    }
}

void Diode::setJunctionCapacitance(double newCap) {
    if (type == Type::Custom) {
        junctionCap = newCap;
    }
}

void Diode::setTransitTime(double newTime) {
    if (type == Type::Custom) {
        transitTime = newTime;
    }
}

double Diode::calculateTemperatureAdjustedIs() const {
    double tempDiff = temperature - 293.15;
    return is * (1.0 + (characteristics.tempCoefIs * tempDiff));
}

double Diode::calculateTemperatureAdjustedVf() const {
    double tempDiff = temperature - 293.15;
    return characteristics.tempCoefVf * tempDiff;
}

double Diode::process(double voltage) {
    lastVoltage = voltage;
    lastCurrent = getCurrent(voltage);
    return lastCurrent;
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
        // Calculate reverse voltage based on current
        return n * vt * std::log(-current / is + 1.0);
    }
}

void Diode::reset() {
    lastVoltage = 0.0;
    lastCurrent = 0.0;
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
    // Simplified reverse current model with breakdown
    if (voltage < -breakdown) {
        // Breakdown region - simplified model
        return -is * (1.0 - std::exp(voltage / (n * calculateThermalVoltage()))) 
               * (1.0 + (voltage + breakdown) / breakdown);
    }
    return -is * (1.0 - std::exp(voltage / (n * calculateThermalVoltage())));
}

double Diode::calculateTotalCurrent(double voltage) const {
    double vt = calculateThermalVoltage();
    
    // Use full Shockley equation for all cases
    double current = is * (std::exp(voltage / (n * vt)) - 1.0);
    
    // Account for series resistance
    if (std::abs(current) > 0) {
        double voltageDrop = current * rs;
        voltage -= voltageDrop;
        // Recalculate current with voltage drop
        current = is * (std::exp(voltage / (n * vt)) - 1.0);
    }
    
    return current;
}

double Diode::calculateJunctionCapacitance(double voltage) const {
    // Junction capacitance varies with reverse bias
    if (voltage < 0) {
        // Reverse bias - junction capacitance decreases
        return junctionCap / std::sqrt(1.0 - voltage / breakdown);
    } else {
        // Forward bias - diffusion capacitance dominates
        double current = calculateForwardCurrent(voltage);
        return junctionCap + (transitTime * current) / calculateThermalVoltage();
    }
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 