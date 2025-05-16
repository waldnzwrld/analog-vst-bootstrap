#pragma once

#include <JuceHeader.h>
#include "../Components/Analog/Transistor.h"
#include "../Components/Analog/Potentiometer.h"
#include "../Components/Analog/Resistor.h"
#include "../Components/Analog/Capacitor.h"
#include "../Components/Analog/Diode.h"

namespace ArchitextureStudiosAnalogCore {

class TransistorClipper {
public:
    TransistorClipper();
    ~TransistorClipper() = default;

    void prepare(double sampleRate);
    void processBlock(juce::AudioBuffer<float>& buffer);
    void reset();

    // Circuit parameters
    void setDrive(float drive);  // 0.0 to 1.0
    float processSample(float input);

private:
    // Circuit components
    ArchitextureStudiosAnalogCore::Analog::Transistor transistor1;    // First NPN transistor
    ArchitextureStudiosAnalogCore::Analog::Transistor transistor2;    // Second NPN transistor
    ArchitextureStudiosAnalogCore::Analog::Diode diode;               // Diode for clipping
    ArchitextureStudiosAnalogCore::Analog::Resistor biasResistor;     // Bias resistor (10kΩ)
    ArchitextureStudiosAnalogCore::Analog::Potentiometer drivePot;    // Drive control (100kΩ)
    ArchitextureStudiosAnalogCore::Analog::Capacitor inputCap;        // Input capacitor
    ArchitextureStudiosAnalogCore::Analog::Capacitor outputCap;       // Output capacitor

    // Circuit constants
    static constexpr double VCC = 9.0;           // Supply voltage
    static constexpr double BIAS_RESISTANCE = 10000.0;  // 10k bias resistor
    static constexpr double INPUT_CAP = 0.056e-6;  // 0.056µF input capacitor
    static constexpr double OUTPUT_CAP = 0.047e-6; // 0.047µF output capacitor

    // State variables
    double sampleRate;
    double lastInputSample;
    double lastOutputSample;
};

} // namespace ArchitextureStudiosAnalogCore 