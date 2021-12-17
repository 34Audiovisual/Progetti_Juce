/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OscillatorDSPAudioProcessorEditor::OscillatorDSPAudioProcessorEditor (OscillatorDSPAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(scope);
    setSize (400, 300);
    startTimer (30);
}

OscillatorDSPAudioProcessorEditor::~OscillatorDSPAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void OscillatorDSPAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void OscillatorDSPAudioProcessorEditor::resized()
{
    //scope.setBounds(getBounds());
    scope.setBounds(10, 10, 50, 50);
}
