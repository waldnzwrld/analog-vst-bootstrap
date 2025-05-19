#pragma once

#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class Diode {
public:
    // Physical constants
    static constexpr double k = 1.380649e-23;  // Boltzmann constant
    static constexpr double q = 1.602176634e-19; // Elementary charge

    enum class Type {
        Silicon,    // Standard silicon diode (e.g., 1N914)
        Zener,      // Zener diode
        Germanium,  // Germanium diode (e.g., 1N34A)
        LED,        // Light Emitting Diode
        Schottky,   // Schottky diode
        Custom      // User-defined parameters
    };

    // Structure to hold diode characteristics
    struct Characteristics {
        double defaultIs;           // Default saturation current
        double defaultN;            // Default ideality factor
        double defaultRs;           // Default series resistance
        double defaultBreakdown;    // Default breakdown voltage
        double defaultJunctionCap;  // Default junction capacitance
        double defaultTransitTime;  // Default transit time
        double minTemp;             // Minimum operating temperature (K)
        double maxTemp;             // Maximum operating temperature (K)
        double tempCoefIs;          // Temperature coefficient for Is (%/°C)
        double tempCoefVf;          // Temperature coefficient for forward voltage (mV/°C)
    };

    Diode(Type type = Type::Silicon, double temp = 293.15);
    ~Diode() = default;

    // Process a single sample
    double process(double voltage);
    
    // Get current through diode
    double getCurrent(double voltage) const;
    
    // Get voltage across diode
    double getVoltage(double current) const;
    
    // Configure diode
    void setTemperature(double temp);
    void setType(Type type);
    
    // Set custom parameters (only used for Custom type)
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
    Type getType() const { return type; }
    const Characteristics& getCharacteristics() const { return characteristics; }
    
    // Reset state
    void reset();

private:
    // Physical properties
    Type type;              // Diode type
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
    
    // Characteristics based on type
    Characteristics characteristics;
    
    // Helper functions
    void initializeCharacteristics();
    double calculateThermalVoltage() const;
    double calculateForwardCurrent(double voltage) const;
    double calculateReverseCurrent(double voltage) const;
    double calculateTotalCurrent(double voltage) const;
    double calculateJunctionCapacitance(double voltage) const;
    double calculateTemperatureAdjustedIs() const;
    double calculateTemperatureAdjustedVf() const;
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 