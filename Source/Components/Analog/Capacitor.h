#pragma once

#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class Capacitor {
public:
    Capacitor(double capValue = 1e-6);  // Default 1µF
    ~Capacitor() = default;

    // Set capacitor value
    void setCapacitance(double capValue);
    
    // Set sample rate
    void setSampleRate(double newSampleRate);
    
    // Process a single sample
    double process(double inputVoltage);
    
    // Reset capacitor state
    void reset();
    
    // Get current voltage
    double getVoltage() const;
    
    // Get current through capacitor
    double getCurrent(double inputVoltage) const;

private:
    // Physical properties
    double capacitance;    // in Farads
    double voltage;        // Current voltage across capacitor
    double lastVoltage;    // Previous voltage for numerical integration
    double sampleRate;     // System sample rate
    double dt;            // Time step (1/sampleRate)
    
    // Constants
    static constexpr double MIN_CAPACITANCE = 1e-12;  // 1 pF
    static constexpr double MAX_CAPACITANCE = 1e-3;   // 1 mF
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 