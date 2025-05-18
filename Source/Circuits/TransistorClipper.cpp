#include "TransistorClipper.h"
#include <JuceHeader.h>

namespace ArchitextureStudiosAnalogCore {

TransistorClipper::TransistorClipper()
    : inputCap(INPUT_CAP)
    , diode()
    , transistor1(ArchitextureStudiosAnalogCore::Analog::Transistor::Type::NPN)
    , transistor2(ArchitextureStudiosAnalogCore::Analog::Transistor::Type::NPN)
    , biasResistor(BIAS_RESISTANCE)
    , outputCap(OUTPUT_CAP)
    , drivePot(100000.0)  // 100k pot
    , sampleRate(44100.0)
    , lastInputSample(0.0)
    , lastOutputSample(0.0)
{
    drivePot.setPosition(0.5f);  // Center position
    juce::Logger::writeToLog("TransistorClipper initialized with default settings");
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
    // Log every 1000th sample to avoid flooding the log
    static int sampleCounter = 0;
    if (++sampleCounter % 1000 == 0) {
        juce::Logger::writeToLog("Sample " + juce::String(sampleCounter) + " - Raw input: " + juce::String(input));
    }
    
    // Amplify input to expected level (around 200mV)
    double amplifiedInput = input * 40.0;  // Bring up to ~200mV range
    
    // Log amplified input
    if (++sampleCounter % 1000 == 0) {
        juce::Logger::writeToLog("Amplified input: " + juce::String(amplifiedInput));
    }

     // Calculate frequency from rate of change of signal
    double signalChange = std::abs(amplifiedInput - lastInputSample);

    double frequency = std::max(20.0, (signalChange * sampleRate) / (2.0 * M_PI));  // Minimum 20Hz to avoid division by zero

    // Calculate capacitor impedance at actual signal frequency
    double capImpedance = 1.0 / (2.0 * M_PI * frequency * INPUT_CAP);

     // Get current through capacitor
    double capCurrent = (amplifiedInput - lastInputSample) / capImpedance;
    lastInputSample = amplifiedInput;

// Convert current to voltage using frequency-dependent impedance
    double capVoltage = capCurrent * capImpedance;
    
    // capVoltage should be around 10x input voltage

    
    // Log capacitor state
    if (sampleCounter % 1000 == 0) {
        juce::Logger::writeToLog("Cap current: " + juce::String(capCurrent) + 
            ", Cap voltage: " + juce::String(capVoltage) +
            ", Frequency: " + juce::String(frequency));
    }
    
    // Process through diode to get base voltage
    double diodeCurrent = diode.process(capVoltage);
    double diodeVoltage = diode.getVoltage(diodeCurrent);
    
    // Log diode state
    if (sampleCounter % 1000 == 0) {
        juce::Logger::writeToLog("Diode current: " + juce::String(diodeCurrent) + 
            ", Diode voltage: " + juce::String(diodeVoltage) +
            ", Input voltage: " + juce::String(capVoltage));
    }
    
    // For Darlington pair:
    // 1. First transistor (T2) base-emitter voltage
    // Base is connected to diode output, emitter to T1's base
    double vbe2 = diodeVoltage + capVoltage;  // Diode voltage directly to base
    
    // 2. First transistor collector-emitter voltage
    // Collector receives diode voltage plus VCC passed through Bias resistor
    double vce2 = diodeVoltage + (VCC / BIAS_RESISTANCE);  // Input voltage directly to collector
    
    // 3. Calculate first transistor current
    double transistor2Current = transistor2.calculateCollectorCurrent(vbe2, vce2);
    double transistor2EmitterCurrent = transistor2.calculateEmitterCurrent(vbe2, vce2);
    
    // 4. Second transistor (T1) base-emitter voltage
    // Base is connected to T2's emitter, emitter to ground
    // Calculate vbe1 from T2's emitter current using Ebers-Moll model
    double vbe1 = transistor2.getVt() * std::log(transistor2EmitterCurrent / transistor2.getIs() + 1.0);
    
    // 5. Second transistor collector-emitter voltage
    // Collector receives diode voltage plus VCC passed through Bias resistor
    double vce1 = diodeVoltage + (VCC  / BIAS_RESISTANCE);   // Input voltage directly to collector

    // T1 Emittor goes to ground
    double transistor1Current = transistor1.calculateCollectorCurrent(vbe1, vce1);
    double transistor1EmitterCurrent = transistor1.calculateEmitterCurrent(vbe1, vce1);
    
    // Output from diode goes to a .047ufcapacitor
    double outputVoltage = outputCap.process(diodeVoltage + (VCC / BIAS_RESISTANCE));

    // Then use voltage divider to get the actual pot output
    double potOutput = drivePot.process(0.0, outputVoltage);
    
    // Log pot output
    if (sampleCounter % 1000 == 0) {
        juce::Logger::writeToLog("Pot output: " + juce::String(potOutput) + 
            ", Pot position: " + juce::String(drivePot.getPosition()));
    }
    
    // Check for invalid output
    if (std::isnan(potOutput) || std::isinf(potOutput)) {
        juce::Logger::writeToLog("ERROR: Invalid output detected: " + juce::String(potOutput));
        return 0.0f;
    }
    
    // Log output level with lower threshold
    if (std::abs(potOutput) > 0.1f) {
        juce::Logger::writeToLog("Output signal detected: " + juce::String(potOutput));
    }
    
    
    
    // Capacitor goes to hot output
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