#pragma once

#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class Diode {
public:
    Diode(double is = 1e-12,           // Reverse saturation current
          double n = 1.0,              // Ideality factor
          double rs = 0.1,             // Series resistance
          double temp = 293.15);       // Temperature in Kelvin
    ~Diode() = default;

    // Process a single sample
    double process(double voltage);
    
    // Get current through diode
    double getCurrent(double voltage) const;
    
    // Get voltage across diode
    double getVoltage(double current) const;
    
    // Configure diode
    void setTemperature(double temp);
    void setSaturationCurrent(double is);
    void setIdealityFactor(double n);
    void setSeriesResistance(double rs);
    
    // Reset state
    void reset();

private:
    // Physical properties
    double is;              // Reverse saturation current
    double n;               // Ideality factor (typically 1-2)
    double rs;              // Series resistance
    double temperature;     // Temperature in Kelvin
    
    // Constants
    static constexpr double k = 1.380649e-23;  // Boltzmann constant
    static constexpr double q = 1.602176634e-19; // Elementary charge
    static constexpr double DEFAULT_TEMP = 293.15; // 20°C
    
    // Helper functions
    double calculateThermalVoltage() const;
    double calculateForwardCurrent(double voltage) const;
    double calculateReverseCurrent(double voltage) const;
    double calculateTotalCurrent(double voltage) const;
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 