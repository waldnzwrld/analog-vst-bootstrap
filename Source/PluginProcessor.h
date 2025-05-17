#pragma once

#include <JuceHeader.h>

class ArchitextureStudiosAnalogCoreAudioProcessor : public juce::AudioProcessor
{
public:
    ArchitextureStudiosAnalogCoreAudioProcessor();
    ~ArchitextureStudiosAnalogCoreAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Getter for parameters
    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }

private:
    // Parameters
    juce::AudioProcessorValueTreeState parameters;
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* toneParam = nullptr;
    std::atomic<float>* levelParam = nullptr;

    // Audio processing variables
    float drive = 1.0f;
    float tone = 0.5f;
    float level = 1.0f;

    // Parameter layout creation
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArchitextureStudiosAnalogCoreAudioProcessor)
}; 