#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class ArchitextureStudiosAnalogCoreAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ArchitextureStudiosAnalogCoreAudioProcessorEditor(ArchitextureStudiosAnalogCoreAudioProcessor&);
    ~ArchitextureStudiosAnalogCoreAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    ArchitextureStudiosAnalogCoreAudioProcessor& audioProcessor;

    juce::Slider driveSlider;
    juce::Label driveLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArchitextureStudiosAnalogCoreAudioProcessorEditor)
}; 