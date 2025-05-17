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
    juce::Slider toneSlider;
    juce::Slider levelSlider;

    juce::Label driveLabel;
    juce::Label toneLabel;
    juce::Label levelLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArchitextureStudiosAnalogCoreAudioProcessorEditor)
}; 