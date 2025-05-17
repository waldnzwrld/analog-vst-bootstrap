#pragma once

#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class Resistor {
public:
    Resistor(double resistance = 1000.0);  // Default 1kΩ
    ~Resistor() = default;

    // Set resistance value
    void setResistance(double resistance);
    
    // Process a single sample (calculate current for given voltage)
    double process(double voltage);
    
    // Get current through resistor
    double getCurrent(double voltage) const;
    
    // Get voltage across resistor
    double getVoltage(double current) const;
    
    // Get power dissipation
    double getPower(double voltage) const;
    
    // Reset state (if we add any state variables later)
    void reset();

private:
    // Physical properties
    double resistance;     // in Ohms
    double temperature;    // in Kelvin (for temperature effects)
    
    // Constants
    static constexpr double MIN_RESISTANCE = 1.0;     // 1 Ω
    static constexpr double MAX_RESISTANCE = 1e6;     // 1 MΩ
    static constexpr double DEFAULT_TEMPERATURE = 293.15; // 20°C
    
    // Temperature coefficient (for temperature effects)
    static constexpr double TEMP_COEFFICIENT = 0.0039; // Typical for carbon film
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 