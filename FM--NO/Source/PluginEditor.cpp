/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMAudioProcessorEditor::FMAudioProcessorEditor (FMAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
   
    addAndMakeVisible(index);
    index.setRange(0.0, 5.0);
    index.setValue(2.0);
    index.onValueChange = [this]
    {
        audioProcessor.setIndex(index.getValue());
    };
    
    addAndMakeVisible(carr);
    carr.setRange(50, 5000);
    carr.setValue(273);
    carr.onValueChange = [this]
    {
        audioProcessor.setCarrFreq(carr.getValue());
    };
    
    addAndMakeVisible(mod);
    mod.setRange(0, 2000);
    mod.setValue(207);
    mod.onValueChange = [this]
    {
        audioProcessor.setModFreq(mod.getValue());
    };
    
    
    
    
    setSize (450, 200);
}

FMAudioProcessorEditor::~FMAudioProcessorEditor()
{
}

//==============================================================================
void FMAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
   // g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    g.drawFittedText("Index", index.getX(), index.getBottom() + 20, index.getWidth(), 20, juce::Justification::centred, 1);
    g.drawFittedText("Carrier Freq", carr.getX(), index.getBottom() + 20, carr.getWidth(), 20, juce::Justification::centred, 1);
    g.drawFittedText("Mod Freq", mod.getX(), mod.getBottom() + 20, mod.getWidth(), 20, juce::Justification::centred, 1);
    g.drawFittedText("FM SYNTHESIS", index.getX(), index.getBottom() + 75, index.getWidth(), 20, juce::Justification::centred, 1);
}

void FMAudioProcessorEditor::resized()
{
    index.setBounds(10, 10, 200, 30);
    carr.setBounds(210, 10, 200, 30);
    mod.setBounds(210, 100, 200, 30);
    
}
