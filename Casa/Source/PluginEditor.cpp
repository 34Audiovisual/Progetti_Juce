/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CasaAudioProcessorEditor::CasaAudioProcessorEditor (CasaAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(scene);
    addAndMakeVisible(scena2);
    setSize (400, 300);
}

CasaAudioProcessorEditor::~CasaAudioProcessorEditor()
{
}

//==============================================================================
void CasaAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void CasaAudioProcessorEditor::resized()
{
    scene.setBounds(getBounds());
    scena2.setBounds(0, 100, 100, 100);
}
