#include "TransistorClipper.h"
#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {

TransistorClipper::TransistorClipper()
    : inputCap(ArchitextureStudiosAnalogCore::Analog::Capacitor::Type::Film, INPUT_CAP, TEMPERATURE)  // Film cap, 0.056µF
    , diode(ArchitextureStudiosAnalogCore::Analog::Diode::Type::Silicon, TEMPERATURE)  // 1N914 parameters
    , transistor1(ArchitextureStudiosAnalogCore::Analog::Transistor::Type::NPN, 
                 400.0f,    // beta
                 0.026f,    // vt
                 1e-12f,    // is
                 100.0f,    // va
                 293.15f,   // temp
                 1e-12f,    // cbc
                 100.0f,    // rb
                 1.0f,      // rc
                 0.1f,      // re
                 0.15f)     // vceSat - 2N5088 typical saturation voltage
    , transistor2(ArchitextureStudiosAnalogCore::Analog::Transistor::Type::NPN,
                 400.0f,    // beta
                 0.026f,    // vt
                 1e-12f,    // is
                 100.0f,    // va
                 293.15f,   // temp
                 1e-12f,    // cbc
                 100.0f,    // rb
                 1.0f,      // rc
                 0.1f,      // re
                 0.15f)     // vceSat - 2N5088 typical saturation voltage
    , biasResistor(BIAS_RESISTANCE, 0.0039, 0.25, 0.5e-12, 0.1e-9)  // Carbon film resistor parameters
    , outputCap(ArchitextureStudiosAnalogCore::Analog::Capacitor::Type::Film, OUTPUT_CAP, TEMPERATURE)  // Film cap, 0.047µF
    , drivePot(100000.0, 0.5, ArchitextureStudiosAnalogCore::Analog::Potentiometer::TaperType::Logarithmic)  // 100k log pot
    , sampleRate(44100.0)
    , lastInputSample(0.0)
    , lastOutputSample(0.0)
{
    drivePot.setPosition(0.5f);  // Center position
    juce::Logger::writeToLog("TransistorClipper initialized with 2N5088 transistors and 1N914 diode");
}

void TransistorClipper::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;
    inputCap.setSampleRate(sampleRate);
    outputCap.setSampleRate(sampleRate);
    reset();
    juce::Logger::writeToLog("TransistorClipper prepared with sample rate: " + juce::String(sampleRate));
}

void TransistorClipper::reset()
{
    lastInputSample = 0.0;
    lastOutputSample = 0.0;
    inputCap.reset();
    outputCap.reset();
    juce::Logger::writeToLog("TransistorClipper reset - all states cleared");
}

void TransistorClipper::setDrive(float drive)
{
    drivePot.setPosition(drive);
    juce::Logger::writeToLog("Drive set to: " + juce::String(drive));
}

float TransistorClipper::processSample(float input)
{
    // Calculate frequency from signal (or use Nyquist for now)
    double frequency = sampleRate / 2.0;  // Nyquist frequency for now
    
    // Amplify input
    double amplifiedInput = input * GAIN;
    
    // Process through input capacitor with frequency
    double capVoltage = inputCap.process(amplifiedInput, frequency);
    
    // Process through diode (now reverse biased)
    double diodeCurrent = diode.process(-capVoltage);  // Negative voltage for reverse bias
    double diodeVoltage = -diode.getVoltage(diodeCurrent);  // Invert the voltage
    
    // Calculate bias current (adjusted for reverse bias)
    double biasCurrent = (VCC + diodeVoltage) / BIAS_RESISTANCE;
    
    // Calculate collector voltages
    double vce2 = VCC - (biasCurrent * BIAS_RESISTANCE);
    double vce1 = VCC - (biasCurrent * BIAS_RESISTANCE);
    
    // Process transistors
    double vbe2 = diodeVoltage + capVoltage;  // Base-emitter voltage for Q2
    double transistor2Current = transistor2.calculateCollectorCurrent(vbe2, vce2);
    double transistor2EmitterCurrent = transistor2.calculateEmitterCurrent(vbe2, vce2);
    
    double vbe1 = transistor2.getVt() * std::log(transistor2EmitterCurrent / transistor2.getIs() + 1.0);
    double transistor1Current = transistor1.calculateCollectorCurrent(vbe1, vce1);
    
    // Process output capacitor with frequency
    double outputVoltage = outputCap.process(diodeVoltage + (VCC / BIAS_RESISTANCE), frequency);
    
    // Process through drive pot
    double potOutput = drivePot.process(0.0, outputVoltage);
    potOutput = std::clamp(potOutput, -VCC, VCC);
    
    return static_cast<float>(potOutput);
}

void TransistorClipper::processBlock(juce::AudioBuffer<float>& buffer)
{
    static int blockCounter = 0;
    juce::Logger::writeToLog("Processing block " + juce::String(++blockCounter) + 
        ": " + juce::String(buffer.getNumSamples()) + " samples, " +
        juce::String(buffer.getNumChannels()) + " channels");
    
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        const float* inputData = buffer.getReadPointer(channel);
        float* outputData = buffer.getWritePointer(channel);
        
        // Log channel statistics
        float maxInput = 0.0f;
        float maxOutput = 0.0f;
        float rmsInput = 0.0f;
        float rmsOutput = 0.0f;
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Process each sample and write to output
            outputData[sample] = processSample(inputData[sample]);
            
            // Track max levels
            maxInput = std::max(maxInput, std::abs(inputData[sample]));
            maxOutput = std::max(maxOutput, std::abs(outputData[sample]));
            
            // Track RMS levels
            rmsInput += inputData[sample] * inputData[sample];
            rmsOutput += outputData[sample] * outputData[sample];
        }
        
        // Calculate RMS
        rmsInput = std::sqrt(rmsInput / buffer.getNumSamples());
        rmsOutput = std::sqrt(rmsOutput / buffer.getNumSamples());
        
        juce::Logger::writeToLog("Channel " + juce::String(channel) + 
            " - Max input: " + juce::String(maxInput) + 
            ", Max output: " + juce::String(maxOutput) +
            ", RMS input: " + juce::String(rmsInput) +
            ", RMS output: " + juce::String(rmsOutput));
    }
}

} // namespace ArchitextureStudiosAnalogCore 