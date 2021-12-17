/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TubeDistAudioProcessorEditor::TubeDistAudioProcessorEditor (TubeDistAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    inGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    inGainSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    inGainSlider.setRange(-48.0, 48.0, 0.0);
    inGainSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(inGainSlider);
    inGainSlider.onValueChange = [this]
    {
        audioProcessor.tubeSett().setInGain(inGainSlider.getValue());
    };
    
    outGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    outGainSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    outGainSlider.setRange(-24.0, 12.0, 0.0);
    outGainSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(outGainSlider);
    outGainSlider.onValueChange = [this]
    {
        audioProcessor.tubeSett().setOutGain(outGainSlider.getValue());
    };
    
    driveSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    driveSlider.setRange(0.0, 10.0, 0.0);
    driveSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(driveSlider);
    driveSlider.onValueChange = [this]
    {
        audioProcessor.tubeSett().setDrive(driveSlider.getValue());
    };
    
    blendSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    blendSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    blendSlider.setRange(-10.0, 10.0, 0.0);
    blendSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(blendSlider);
    blendSlider.onValueChange = [this]
    {
        audioProcessor.tubeSett().setBlend(blendSlider.getValue());
    };
    
    setSize (500, 250);
}

TubeDistAudioProcessorEditor::~TubeDistAudioProcessorEditor()
{
}

//==============================================================================
void TubeDistAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("In Gain", inGainSlider.getX(), inGainSlider.getBottom(), inGainSlider.getWidth(), 20, juce::Justification::centred, 1);
    g.drawFittedText("Out Gain", outGainSlider.getX(), outGainSlider.getBottom(), outGainSlider.getWidth(), 20, juce::Justification::centred, 1);
    g.drawFittedText("Drive", driveSlider.getX(), driveSlider.getBottom(), driveSlider.getWidth(), 20, juce::Justification::centred, 1);
    g.drawFittedText("Blend(Tube/Tape)", blendSlider.getX(), blendSlider.getBottom(), blendSlider.getWidth(), 20, juce::Justification::centred, 1);
    g.setFont (25.0f);
    g.drawFittedText("AnalogDirtyDie (by Diego Capoccitti @2021)", 0, 200, getWidth(), 20, juce::Justification::centred, 1);
}

void TubeDistAudioProcessorEditor::resized()
{
    int border = 10;
    int dimSlider = (int)((getWidth() - border) / 4);
    
    
    inGainSlider.setBounds(border, border, dimSlider, dimSlider);
    outGainSlider.setBounds(inGainSlider.getRight(), border, dimSlider, dimSlider);
    driveSlider.setBounds(outGainSlider.getRight(), border, dimSlider, dimSlider);
    blendSlider.setBounds(driveSlider.getRight(), border, dimSlider, dimSlider);
    
}
