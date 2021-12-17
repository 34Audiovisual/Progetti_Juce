/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DualOscillatorAudioProcessorEditor::DualOscillatorAudioProcessorEditor (DualOscillatorAudioProcessor& p)
    : AudioProcessorEditor (&p), ui(p.apvts), audioProcessor (p)
{
    startTimer(30);
    addAndMakeVisible(ui);
    setSize (518, 680);
}

DualOscillatorAudioProcessorEditor::~DualOscillatorAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void DualOscillatorAudioProcessorEditor::paint (juce::Graphics& g)
{

}

void DualOscillatorAudioProcessorEditor::resized()
{
    ui.setBounds(getBounds());
}
