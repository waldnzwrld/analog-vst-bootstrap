#pragma once

#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class Capacitor {
public:
    Capacitor(double capValue = 1e-6, double esr = 0.0, double esl = 0.0, double temp = 293.15);  // Default 1µF, 20°C
    ~Capacitor() = default;

    // Set capacitor value
    void setCapacitance(double capValue);
    
    // Set ESR and ESL
    void setESR(double esr);
    void setESL(double esl);
    
    // Set temperature (in Kelvin)
    void setTemperature(double temp);
    
    // Get ESR and ESL
    double getESR() const { return esr; }
    double getESL() const { return esl; }
    double getTemperature() const { return temperature; }
    
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
    double esr;           // Equivalent Series Resistance in Ohms
    double esl;           // Equivalent Series Inductance in Henries
    double temperature;   // Temperature in Kelvin
    double voltage;        // Current voltage across capacitor
    double lastVoltage;    // Previous voltage for numerical integration
    double lastCurrent;    // Previous current for ESL calculation
    double sampleRate;     // System sample rate
    double dt;            // Time step (1/sampleRate)
    
    // Temperature coefficients (typical for film capacitors)
    static constexpr double TEMP_COEF_CAP = -150e-6;  // -150ppm/°C
    static constexpr double TEMP_COEF_ESR = 0.002;    // 0.2%/°C
    
    // Constants
    static constexpr double MIN_CAPACITANCE = 1e-12;  // 1 pF
    static constexpr double MAX_CAPACITANCE = 1e-3;   // 1 mF
    static constexpr double MIN_ESR = 0.0;           // 0 Ohms
    static constexpr double MAX_ESR = 100.0;         // 100 Ohms
    static constexpr double MIN_ESL = 0.0;           // 0 Henries
    static constexpr double MAX_ESL = 1e-6;          // 1 µH
    static constexpr double MIN_TEMP = 233.15;       // -40°C
    static constexpr double MAX_TEMP = 373.15;       // 100°C
    
    // Helper methods
    double calculateTemperatureAdjustedCapacitance() const;
    double calculateTemperatureAdjustedESR() const;
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 