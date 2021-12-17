/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OscillatoreSinAudioProcessorEditor::OscillatoreSinAudioProcessorEditor (OscillatoreSinAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    freqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(freqSlider);
    //freqSlider.setRange (50.0, 10000.0);
    //freqSlider.setSkewFactorFromMidPoint (500.0);
    freqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FREQ", freqSlider);
    freqSlider.onValueChange = [this]
    {
        audioProcessor.changeFreq();

    };
    
    setSize (400, 300);
}

OscillatoreSinAudioProcessorEditor::~OscillatoreSinAudioProcessorEditor()
{
}

//==============================================================================
void OscillatoreSinAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void OscillatoreSinAudioProcessorEditor::resized()
{
    freqSlider.setBounds(100, 100, 200, 200);
}
