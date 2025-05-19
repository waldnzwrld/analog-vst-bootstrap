#pragma once

#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class Resistor {
public:
    Resistor(double resistance = 1000.0,           // Nominal resistance
             double tempCoeff = 0.0039,            // Temperature coefficient (typical for carbon film)
             double powerRating = 0.25,            // Power rating in watts
             double parasiticCap = 0.5e-12,        // Parasitic capacitance
             double parasiticInd = 0.1e-9);        // Parasitic inductance

    ~Resistor() = default;

    // Set resistance value
    void setResistance(double resistance);
    
    // Set temperature
    void setTemperature(double temp);
    
    // Process a single sample (calculate current for given voltage)
    double process(double voltage);
    
    // Get current through resistor
    double getCurrent(double voltage) const;
    
    // Get voltage across resistor
    double getVoltage(double current, double frequency = 0.0) const;
    
    // Get power dissipation
    double getPower(double voltage) const;
    
    // Get thermal noise voltage
    double getThermalNoise() const;
    
    // Get actual resistance including temperature and power effects
    double getActualResistance() const;
    
    // Reset state
    void reset();

private:
    // Physical properties
    double nominalResistance;  // Nominal resistance value
    double actualResistance;   // Actual resistance including effects
    double tempCoeff;         // Temperature coefficient
    double powerRating;       // Power rating in watts
    double parasiticCap;      // Parasitic capacitance
    double parasiticInd;      // Parasitic inductance
    double temperature;       // Current temperature in Kelvin
    double powerDissipation;  // Current power dissipation
    
    // State variables
    double lastVoltage;       // Previous voltage for stability
    double lastCurrent;       // Previous current for stability
    
    // Constants
    static constexpr double MIN_RESISTANCE = 1.0;     // 1 Ω
    static constexpr double MAX_RESISTANCE = 1e6;     // 1 MΩ
    static constexpr double DEFAULT_TEMPERATURE = 293.15; // 20°C
    static constexpr double MIN_TEMPERATURE = 233.15;  // -40°C
    static constexpr double MAX_TEMPERATURE = 373.15;  // 100°C
    static constexpr double BOLTZMANN_CONSTANT = 1.380649e-23; // J/K
    
    // Helper functions
    void updateActualResistance();
    double calculateTemperatureEffect() const;
    double calculatePowerEffect() const;
    double calculateFrequencyEffect(double frequency) const;
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 