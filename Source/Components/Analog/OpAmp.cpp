// Source/Components/Analog/OpAmp.cpp
#include "OpAmp.h"

namespace ArchitextureStudiosAnalogCore {
namespace Analog {

OpAmp::OpAmp(double gain, double slewRate, double bandwidth) :
    gain(gain),
    slewRate(slewRate),
    bandwidth(bandwidth),
    inputImpedance(DEFAULT_INPUT_IMPEDANCE),
    outputImpedance(DEFAULT_OUTPUT_IMPEDANCE),
    lastOutput(0.0),
    sampleRate(44100.0),
    dt(1.0/44100.0),
    vPlus(0.0),
    vMinus(0.0),
    vOut(0.0)
{
}

double OpAmp::process(double inPlus, double inMinus) {
    // Store input voltages
    vPlus = inPlus;
    vMinus = inMinus;
    
    // Calculate differential input
    double diffInput = vPlus - vMinus;
    
    // Apply gain
    double amplified = diffInput * gain;
    
    // Apply frequency response
    amplified = applyFrequencyResponse(amplified);
    
    // Apply slew rate limiting
    amplified = limitSlewRate(amplified);
    
    // Apply saturation
    vOut = saturate(amplified);
    
    // Store output for next iteration
    lastOutput = vOut;
    
    return vOut;
}

void OpAmp::setGain(double newGain) {
    gain = newGain;
}

void OpAmp::setSlewRate(double newSlewRate) {
    slewRate = newSlewRate;
}

void OpAmp::setBandwidth(double newBandwidth) {
    bandwidth = newBandwidth;
}

void OpAmp::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
    dt = 1.0 / sampleRate;
}

void OpAmp::reset() {
    lastOutput = 0.0;
    vPlus = 0.0;
    vMinus = 0.0;
    vOut = 0.0;
}

double OpAmp::getOutput() const {
    return vOut;
}

double OpAmp::getInputOffset() const {
    return vPlus - vMinus;
}

double OpAmp::limitSlewRate(double newOutput) {
    double maxChange = slewRate * dt * 1e6; // Convert V/µs to V/sample
    double change = newOutput - lastOutput;
    
    if (std::abs(change) > maxChange) {
        return lastOutput + (change > 0 ? maxChange : -maxChange);
    }
    return newOutput;
}

double OpAmp::saturate(double input) {
    // Simple saturation to ±15V rails
    return std::clamp(input, -15.0, 15.0);
}

double OpAmp::applyFrequencyResponse(double input) {
    // Simple single-pole response
    double pole = 2.0 * M_PI * bandwidth / gain;
    double alpha = pole * dt;
    return (input + alpha * lastOutput) / (1.0 + alpha);
}

} // namespace Analog
} // namespace ArchitextureStudiosAnalogCore