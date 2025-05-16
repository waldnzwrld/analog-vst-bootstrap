// Source/Components/Analog/OpAmp.h
#pragma once

#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

class OpAmp {
public:
    OpAmp(double gain = DEFAULT_GAIN,
          double slewRate = DEFAULT_SLEW_RATE,
          double bandwidth = DEFAULT_BANDWIDTH);
    ~OpAmp() = default;

    // Process a single sample
    double process(double inPlus, double inMinus);
    
    // Configure op-amp
    void setGain(double newGain);
    void setSlewRate(double newSlewRate);
    void setBandwidth(double newBandwidth);
    void setSampleRate(double newSampleRate);
    
    // Reset state
    void reset();
    
    // Get current state
    double getOutput() const;
    double getInputOffset() const;

private:
    // Physical properties
    double gain;              // Open-loop gain (typically 100,000+)
    double slewRate;          // Slew rate in V/µs
    double bandwidth;         // Unity gain bandwidth in Hz
    double inputImpedance;    // Input impedance in ohms
    double outputImpedance;   // Output impedance in ohms
    
    // State variables
    double lastOutput;        // Previous output for slew rate limiting
    double sampleRate;        // System sample rate
    double dt;               // Time step (1/sampleRate)
    
    // Internal nodes
    double vPlus;            // Non-inverting input
    double vMinus;           // Inverting input
    double vOut;             // Output voltage
    
    // Constants
    static constexpr double DEFAULT_GAIN = 100000.0;        // 100dB
    static constexpr double DEFAULT_SLEW_RATE = 0.5;        // 0.5 V/µs
    static constexpr double DEFAULT_BANDWIDTH = 1000000.0;  // 1 MHz
    static constexpr double DEFAULT_INPUT_IMPEDANCE = 1e6;  // 1 MΩ
    static constexpr double DEFAULT_OUTPUT_IMPEDANCE = 75.0; // 75 Ω
    
    // Helper functions
    double limitSlewRate(double newOutput);
    double applyFrequencyResponse(double input);
    double saturate(double input);
};

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore