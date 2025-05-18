#include "PluginProcessor.h"
#include "PluginEditor.h"

ArchitextureStudiosAnalogCoreAudioProcessorEditor::ArchitextureStudiosAnalogCoreAudioProcessorEditor(ArchitextureStudiosAnalogCoreAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set up drive slider
    driveSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 30);
    driveSlider.setRange(0.0f, 1.0f, 0.01f);
    driveSlider.setValue(0.5f);
    addAndMakeVisible(driveSlider);
    driveAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getParameters(), "drive", driveSlider));

    // Set up label
    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(driveLabel);

    // Set window size
    setSize(200, 200);
}

ArchitextureStudiosAnalogCoreAudioProcessorEditor::~ArchitextureStudiosAnalogCoreAudioProcessorEditor()
{
}

void ArchitextureStudiosAnalogCoreAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("BassFukz", getLocalBounds(), juce::Justification::centredTop, true);
}

void ArchitextureStudiosAnalogCoreAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop(40); // Space for title

    const int sliderWidth = 120;
    const int sliderHeight = 120;
    const int labelHeight = 20;

    // Center the drive control
    driveLabel.setBounds(area.getCentreX() - sliderWidth/2, area.getY(), sliderWidth, labelHeight);
    driveSlider.setBounds(area.getCentreX() - sliderWidth/2, area.getY() + labelHeight, sliderWidth, sliderHeight);
} 