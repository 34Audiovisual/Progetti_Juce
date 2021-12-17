/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AllPassbiquadAudioProcessorEditor::AllPassbiquadAudioProcessorEditor (AllPassbiquadAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    frequency.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    frequency.setRange(0, 5000);
    frequency.onValueChange = [this]
    {
        audioProcessor.setFreq(frequency.getValue());
    };
    frequency.setSkewFactor(0.2);
    addAndMakeVisible(frequency);
    
    setSize (250, 250);
}

AllPassbiquadAudioProcessorEditor::~AllPassbiquadAudioProcessorEditor()
{
}

//==============================================================================
void AllPassbiquadAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("Frequency Shifter", 0, getBottom() - 20, getWidth(), 20, juce::Justification::centred, 1);
}

void AllPassbiquadAudioProcessorEditor::resized()
{
    frequency.setBounds(10, 10, 200, 200);
}
