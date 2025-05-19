#pragma once

#include <JuceHeader.h>
#include <array>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class Capacitor {
public:
    enum class Type {
        Film,           // Polyester, polypropylene, etc.
        Ceramic,        // X7R, NP0, etc.
        Electrolytic,   // Aluminum, tantalum
        Mica,          // Silver mica
        Paper,         // Oil-filled paper
        Custom         // User-defined parameters
    };

    // Structure to hold capacitor characteristics
    struct Characteristics {
        double defaultESR;           // Default ESR in Ohms
        double defaultESL;           // Default ESL in Henries
        double dielectricAbsorption; // DA as percentage (0.0 to 1.0)
        double tempCoefCap;          // Temperature coefficient for capacitance (ppm/°C)
        double tempCoefESR;          // Temperature coefficient for ESR (%/°C)
        double maxVoltage;           // Maximum voltage rating
        double minTemp;              // Minimum operating temperature (K)
        double maxTemp;              // Maximum operating temperature (K)
    };

    Capacitor(Type type = Type::Film, double capValue = 1e-6, double temp = 293.15);
    ~Capacitor() = default;

    // Set capacitor value
    void setCapacitance(double capValue);
    
    // Set ESR and ESL (only used for Custom type)
    void setESR(double esr);
    void setESL(double esl);
    
    // Set temperature (in Kelvin)
    void setTemperature(double temp);
    
    // Set type and update characteristics
    void setType(Type type);
    
    // Get characteristics
    double getESR() const { return esr; }
    double getESL() const { return esl; }
    double getTemperature() const { return temperature; }
    Type getType() const { return type; }
    const Characteristics& getCharacteristics() const { return characteristics; }
    
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
    Type type;           // Capacitor type
    double capacitance;  // in Farads
    double esr;         // Equivalent Series Resistance in Ohms
    double esl;         // Equivalent Series Inductance in Henries
    double temperature; // Temperature in Kelvin
    double voltage;     // Current voltage across capacitor
    double lastVoltage; // Previous voltage for numerical integration
    double lastCurrent; // Previous current for ESL calculation
    double sampleRate;  // System sample rate
    double dt;         // Time step (1/sampleRate)
    
    // Characteristics based on type
    Characteristics characteristics;
    
    // Dielectric absorption properties
    static constexpr size_t DA_HISTORY_SIZE = 8;  // Number of time constants to track
    std::array<double, DA_HISTORY_SIZE> daVoltages;  // Voltage history for DA
    std::array<double, DA_HISTORY_SIZE> daTimeConstants;  // Time constants for different DA mechanisms
    
    // Constants
    static constexpr double MIN_CAPACITANCE = 1e-12;  // 1 pF
    static constexpr double MAX_CAPACITANCE = 1e-3;   // 1 mF
    
    // Helper methods
    void initializeCharacteristics();
    double calculateTemperatureAdjustedCapacitance() const;
    double calculateTemperatureAdjustedESR() const;
    double calculateDielectricAbsorptionVoltage() const;
    void updateDAVoltages(double newVoltage);
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 