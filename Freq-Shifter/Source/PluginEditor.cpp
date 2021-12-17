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
    frequency.setRange(0, 5000, 1);
    frequency.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 250, 25);
    frequency.setTextBoxIsEditable(true);
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
    g.drawFittedText("Frequency Shifter [Hz]", 0, getBottom() - 20, getWidth(), 20, juce::Justification::centred, 1);
}

void AllPassbiquadAudioProcessorEditor::resized()
{
    frequency.setBounds(20, 20, getWidth() - 40, getWidth()-40);
}
