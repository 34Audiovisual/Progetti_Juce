/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (SamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mLoadButton.onClick = [&] { audioProcessor.loadFile();};
    addAndMakeVisible(mLoadButton);
    
    addAndMakeVisible(loadedLabel);
    loadedLabel.setText("File Loaded", juce::dontSendNotification);
    loadedLabel.setJustificationType(juce::Justification::centred);
    loadedLabel.setVisible(false);
    
    setSize (200, 200);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
}

//==============================================================================
void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    if (audioProcessor.getNumSound() > 0)
        loadedLabel.setVisible(true);
    else
        loadedLabel.setVisible(false);
}

void SamplerAudioProcessorEditor::resized()
{
    mLoadButton.setBounds(getBounds());
    loadedLabel.setBounds(0, getBottom() - 25, getWidth(), 25);
}
