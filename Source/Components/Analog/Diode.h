#pragma once

#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class Diode {
public:
    // Physical constants
    static constexpr double k = 1.380649e-23;  // Boltzmann constant
    static constexpr double q = 1.602176634e-19; // Elementary charge

    Diode(double is = 1e-12,           // Reverse saturation current
          double n = 1.0,              // Ideality factor
          double rs = 0.1,             // Series resistance
          double temp = 293.15,        // Temperature in Kelvin
          double breakdown = 100.0,    // Breakdown voltage
          double junctionCap = 1e-12,  // Junction capacitance
          double transitTime = 1e-9);  // Transit time

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
    void setBreakdownVoltage(double breakdown);
    void setJunctionCapacitance(double cap);
    void setTransitTime(double time);
    
    // Get diode parameters
    double getTemperature() const { return temperature; }
    double getSaturationCurrent() const { return is; }
    double getIdealityFactor() const { return n; }
    double getSeriesResistance() const { return rs; }
    double getBreakdownVoltage() const { return breakdown; }
    double getJunctionCapacitance() const { return junctionCap; }
    double getTransitTime() const { return transitTime; }
    
    // Reset state
    void reset();

private:
    // Physical properties
    double is;              // Reverse saturation current
    double n;               // Ideality factor (typically 1-2)
    double rs;              // Series resistance
    double temperature;     // Temperature in Kelvin
    double breakdown;       // Breakdown voltage
    double junctionCap;     // Junction capacitance
    double transitTime;     // Transit time
    
    // State variables
    double lastVoltage;     // Last voltage across diode
    double lastCurrent;     // Last current through diode
    
    // Helper functions
    double calculateThermalVoltage() const;
    double calculateForwardCurrent(double voltage) const;
    double calculateReverseCurrent(double voltage) const;
    double calculateTotalCurrent(double voltage) const;
    double calculateJunctionCapacitance(double voltage) const;
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 