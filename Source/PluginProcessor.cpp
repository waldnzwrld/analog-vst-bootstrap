#include "PluginProcessor.h"
#include "PluginEditor.h"

PedalPowerAudioProcessor::PedalPowerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , parameters(*this, nullptr, "Parameters", createParameterLayout())
{
    driveParam = parameters.getRawParameterValue("drive");
    toneParam = parameters.getRawParameterValue("tone");
    levelParam = parameters.getRawParameterValue("level");
}

PedalPowerAudioProcessor::~PedalPowerAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout PedalPowerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("drive", "Drive", 1.0f, 10.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("tone", "Tone", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("level", "Level", 0.0f, 2.0f, 1.0f));

    return { params.begin(), params.end() };
}

void PedalPowerAudioProcessor::prepareToPlay(double /*sampleRate*/, int /*samplesPerBlock*/)
{
    // Initialize any resources needed for processing
}

void PedalPowerAudioProcessor::releaseResources()
{
    // Free any resources that were allocated in prepareToPlay
}

void PedalPowerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    
    // Update parameters
    drive = *driveParam;
    tone = *toneParam;
    level = *levelParam;

    // Process each channel
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        
        // Process each sample
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float input = channelData[sample];
            
            // Apply drive (soft clipping)
            float output = std::tanh(input * drive);
            
            // Apply tone control (simple low-pass filter)
            output = output * (1.0f - tone) + input * tone;
            
            // Apply level
            output *= level;
            
            channelData[sample] = output;
        }
    }
}

juce::AudioProcessorEditor* PedalPowerAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

void PedalPowerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PedalPowerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PedalPowerAudioProcessor();
} 