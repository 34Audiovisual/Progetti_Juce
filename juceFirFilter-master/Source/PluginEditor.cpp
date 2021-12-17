/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JuceFirfilterExampleAudioProcessorEditor::JuceFirfilterExampleAudioProcessorEditor (JuceFirfilterExampleAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 200);
    
    filterCutoffDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    filterCutoffDial.setRange(20.0f, 20000.0f);
    filterCutoffDial.setValue(600.0f);
    filterCutoffDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filterCutoffDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterCutoffDial);
    
    filterCutoffValue = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "cutoff", filterCutoffDial);
    
    filterCutoffDial.setSkewFactorFromMidPoint(1000.0f);
}

JuceFirfilterExampleAudioProcessorEditor::~JuceFirfilterExampleAudioProcessorEditor()
{
}

//==============================================================================
void JuceFirfilterExampleAudioProcessorEditor::paint (Graphics& g)
{
    //background ui stuff
    Rectangle<int> titleArea (0, 10, getWidth(), 20);
    
    g.fillAll (Colours::black);
    g.setColour(Colours::white);
    g.drawText("FIR Low Pass Filter", titleArea, Justification::centredTop);
    
    Rectangle <float> area (25, 25, 150, 150);
    
    g.setColour(Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void JuceFirfilterExampleAudioProcessorEditor::resized()
{
    //need to come back and dynamically set these...ok for now
    Rectangle<int> area = getLocalBounds().reduced(40);
    
    filterCutoffDial.setBounds (area.getWidth() - 90, area.getHeight() - 90, 140, 140);
}
