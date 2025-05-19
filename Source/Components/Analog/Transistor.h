#pragma once

#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class Transistor
{
public:
    enum class Type
    {
        NPN,
        PNP
    };

    // Physical constants
    static constexpr float k = 1.380649e-23f;  // Boltzmann constant
    static constexpr float q = 1.602176634e-19f; // Elementary charge

    Transistor(Type type,
              float beta = 100.0f,           // Current gain (hFE)
              float vt = 0.026f,            // Thermal voltage (26mV at room temperature)
              float is = 1e-12f,            // Saturation current
              float va = 100.0f,            // Early voltage
              float temp = 293.15f,         // Temperature in Kelvin
              float cbc = 1e-12f,           // Base-collector capacitance
              float rb = 100.0f,            // Base resistance
              float rc = 1.0f,              // Collector resistance
              float re = 0.1f);             // Emitter resistance

    ~Transistor() = default;

    // Set transistor parameters
    void setBeta(float newBeta);        // Current gain (hFE)
    void setVt(float newVt);           // Thermal voltage
    void setIs(float newIs);           // Saturation current
    void setEarlyVoltage(float newVa); // Early voltage
    void setTemperature(float temp);    // Temperature in Kelvin
    void setMillerCap(float newCbc);   // Base-collector capacitance
    void setBaseResistance(float newRb);// Base resistance
    void setCollectorResistance(float newRc); // Collector resistance
    void setEmitterResistance(float newRe);   // Emitter resistance

    // Get transistor parameters
    float getBeta() const { return beta; }
    float getVt() const { return vt; }
    float getIs() const { return is; }
    float getEarlyVoltage() const { return va; }
    float getTemperature() const { return temperature; }
    float getMillerCap() const { return cbc; }
    float getBaseResistance() const { return rb; }
    float getCollectorResistance() const { return rc; }
    float getEmitterResistance() const { return re; }
    Type getType() const { return type; }

    // Calculate collector current using Ebers-Moll model
    float calculateCollectorCurrent(float vbe, float vce);

    // Calculate emitter current (Ic + Ib)
    float calculateEmitterCurrent(float vbe, float vce);

    // Calculate base current
    float calculateBaseCurrent(float vbe, float vce);

    // Calculate Miller effect capacitance
    float calculateMillerCapacitance(float vce, float gain);

    // Reset state
    void reset();
  
private:
    Type type;
    float beta;    // Current gain (hFE)
    float vt;      // Thermal voltage (typically 26mV at room temperature)
    float is;      // Saturation current
    float va;      // Early voltage
    float temperature; // Temperature in Kelvin
    float cbc;     // Base-collector capacitance (Miller capacitance)
    float rb;      // Base resistance
    float rc;      // Collector resistance
    float re;      // Emitter resistance
    float lastVbe; // Last base-emitter voltage
    float lastVce; // Last collector-emitter voltage
    float lastIc;  // Last collector current
    float lastIb;  // Last base current
    float lastIe;  // Last emitter current

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Transistor)
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 