#include "PluginProcessor.h"
#include "PluginEditor.h"

ArchitextureStudiosAnalogCoreAudioProcessor::ArchitextureStudiosAnalogCoreAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
    , parameters(*this, nullptr, "Parameters", createParameterLayout())
{
}

ArchitextureStudiosAnalogCoreAudioProcessor::~ArchitextureStudiosAnalogCoreAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout ArchitextureStudiosAnalogCoreAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "drive", "Drive", 0.0f, 1.0f, 0.5f));
    
    return { params.begin(), params.end() };
}

void ArchitextureStudiosAnalogCoreAudioProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    circuit.prepare(sampleRate);
}

void ArchitextureStudiosAnalogCoreAudioProcessor::releaseResources()
{
    circuit.reset();
}

void ArchitextureStudiosAnalogCoreAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    
    auto* driveParam = parameters.getRawParameterValue("drive");
    circuit.setDrive(*driveParam);
    circuit.processBlock(buffer);
}

juce::AudioProcessorEditor* ArchitextureStudiosAnalogCoreAudioProcessor::createEditor()
{
    return new ArchitextureStudiosAnalogCoreAudioProcessorEditor(*this);
}

void ArchitextureStudiosAnalogCoreAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ArchitextureStudiosAnalogCoreAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

bool ArchitextureStudiosAnalogCoreAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ArchitextureStudiosAnalogCoreAudioProcessor();
} 