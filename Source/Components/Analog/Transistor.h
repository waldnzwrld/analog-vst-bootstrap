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

    Transistor(Type type = Type::NPN);
    ~Transistor() = default;

    // Set transistor parameters
    void setBeta(float newBeta);        // Current gain (hFE)
    void setVt(float newVt);           // Thermal voltage
    void setIs(float newIs);           // Saturation current
    void setEarlyVoltage(float newVa); // Early voltage

    // Get transistor parameters
    float getBeta() const { return beta; }
    float getVt() const { return vt; }
    float getIs() const { return is; }
    float getEarlyVoltage() const { return va; }
    Type getType() const { return type; }

    // Calculate collector current using Ebers-Moll model
    float calculateCollectorCurrent(float vbe, float vce);

private:
    Type type;
    float beta;    // Current gain (hFE)
    float vt;      // Thermal voltage (typically 26mV at room temperature)
    float is;      // Saturation current
    float va;      // Early voltage

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Transistor)
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore 