/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OscilloscopeXYAudioProcessorEditor::OscilloscopeXYAudioProcessorEditor (OscilloscopeXYAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(scope);
    startTimer(30);
    setSize (400, 400);
}

OscilloscopeXYAudioProcessorEditor::~OscilloscopeXYAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void OscilloscopeXYAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void OscilloscopeXYAudioProcessorEditor::resized()
{
    scope.setBounds(getBounds().expanded(-10));
}
