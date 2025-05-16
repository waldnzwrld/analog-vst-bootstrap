#include "TransistorClipper.h"
#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {

TransistorClipper::TransistorClipper()
    : inputCap(INPUT_CAP)
    , diode()
    , transistor(ArchitextureStudiosAnalogCore::Analog::Transistor::Type::NPN)
    , biasResistor(BIAS_RESISTANCE)
    , outputCap(OUTPUT_CAP)
    , drivePot(10000.0)  // 10k pot
    , sampleRate(44100.0)
    , lastInputSample(0.0)
    , lastOutputSample(0.0)
{
    drivePot.setPosition(0.5f);  // Center position
}

void TransistorClipper::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;
    inputCap.setSampleRate(sampleRate);
    outputCap.setSampleRate(sampleRate);
    reset();
}

void TransistorClipper::reset()
{
    lastInputSample = 0.0;
    lastOutputSample = 0.0;
    inputCap.reset();
    outputCap.reset();
}

void TransistorClipper::setDrive(float drive)
{
    drivePot.setPosition(drive);
}

float TransistorClipper::processSample(float input)
{
    // Scale input to our expected voltage range
    double inputVoltage = input * INPUT_VPP;
    
    // Process through input capacitor
    double capVoltage = inputCap.process(inputVoltage - lastInputSample);
    lastInputSample = inputVoltage;
    
    // Calculate bias current through 10k resistor
    double biasCurrent = (VCC - capVoltage) / BIAS_RESISTANCE;
    
    // Process through diode and transistor in parallel
    double diodeCurrent = diode.process(capVoltage);
    double transistorCurrent = transistor.calculateCollectorCurrent(capVoltage, VCC);
    
    // Combine currents
    double totalCurrent = biasCurrent + diodeCurrent + transistorCurrent;
    
    // Process through output capacitor
    double outputVoltage = outputCap.process(totalCurrent);
    
    // Scale back to audio range
    return static_cast<float>(outputVoltage * INPUT_VPP);
}

void TransistorClipper::processBlock(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        const float* inputData = buffer.getReadPointer(channel);
        float* outputData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Process each sample and write to output
            outputData[sample] = processSample(inputData[sample]);
        }
    }
}

} // namespace ArchitextureStudiosAnalogCore 