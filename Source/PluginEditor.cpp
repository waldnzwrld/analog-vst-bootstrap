#include "PluginProcessor.h"
#include "PluginEditor.h"

ArchitextureStudiosAnalogCoreAudioProcessorEditor::ArchitextureStudiosAnalogCoreAudioProcessorEditor(ArchitextureStudiosAnalogCoreAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set up drive slider
    driveSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 30);
    driveSlider.setRange(1.0f, 10.0f, 0.1f);
    driveSlider.setValue(1.0f);
    addAndMakeVisible(driveSlider);
    driveAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getParameters(), "drive", driveSlider));

    // Set up tone slider
    toneSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 30);
    toneSlider.setRange(0.0f, 1.0f, 0.01f);
    toneSlider.setValue(0.5f);
    addAndMakeVisible(toneSlider);
    toneAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getParameters(), "tone", toneSlider));

    // Set up level slider
    levelSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    levelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 30);
    levelSlider.setRange(0.0f, 2.0f, 0.01f);
    levelSlider.setValue(1.0f);
    addAndMakeVisible(levelSlider);
    levelAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getParameters(), "level", levelSlider));

    // Set up labels
    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(driveLabel);

    toneLabel.setText("Tone", juce::dontSendNotification);
    toneLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(toneLabel);

    levelLabel.setText("Level", juce::dontSendNotification);
    levelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(levelLabel);

    // Set window size
    setSize(400, 300);
}

ArchitextureStudiosAnalogCoreAudioProcessorEditor::~ArchitextureStudiosAnalogCoreAudioProcessorEditor()
{
}

void ArchitextureStudiosAnalogCoreAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Pedal Power", getLocalBounds(), juce::Justification::centredTop, true);
}

void ArchitextureStudiosAnalogCoreAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop(40); // Space for title

    const int sliderWidth = 100;
    const int sliderHeight = 100;
    const int labelHeight = 20;

    // Drive control
    driveLabel.setBounds(area.getX(), area.getY(), sliderWidth, labelHeight);
    driveSlider.setBounds(area.getX(), area.getY() + labelHeight, sliderWidth, sliderHeight);

    // Tone control
    toneLabel.setBounds(area.getCentreX() - sliderWidth/2, area.getY(), sliderWidth, labelHeight);
    toneSlider.setBounds(area.getCentreX() - sliderWidth/2, area.getY() + labelHeight, sliderWidth, sliderHeight);

    // Level control
    levelLabel.setBounds(area.getRight() - sliderWidth, area.getY(), sliderWidth, labelHeight);
    levelSlider.setBounds(area.getRight() - sliderWidth, area.getY() + labelHeight, sliderWidth, sliderHeight);
} 