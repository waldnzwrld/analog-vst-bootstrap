#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class PedalPowerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    PedalPowerAudioProcessorEditor(PedalPowerAudioProcessor&);
    ~PedalPowerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    PedalPowerAudioProcessor& audioProcessor;

    juce::Slider driveSlider;
    juce::Slider toneSlider;
    juce::Slider levelSlider;

    juce::Label driveLabel;
    juce::Label toneLabel;
    juce::Label levelLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PedalPowerAudioProcessorEditor)
}; 