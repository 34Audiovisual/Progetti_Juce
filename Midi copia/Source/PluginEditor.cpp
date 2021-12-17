/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiAudioProcessorEditor::MidiAudioProcessorEditor (MidiAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), messaggioMidi ( audioProcessor.currentMidiMess)
{
    //messaggioMidi = audioProcessor.currentMidiMess;
    addAndMakeVisible(messMidi);
    messMidi.setButtonText("Messaggio MIDI");
    messMidi.onClick = [this]
    {
        ++i;
        audioProcessor.addEvent();
        messaggio = scritta + (juce::String)i + ": " + audioProcessor.currentMidiMess;
        repaint();
    };
    
    
    setSize (400, 300);
}

MidiAudioProcessorEditor::~MidiAudioProcessorEditor()
{
}

//==============================================================================
void MidiAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText (messaggio, getLocalBounds(), juce::Justification::centred, 1);
}

void MidiAudioProcessorEditor::resized()
{
    messMidi.setBounds(20, 20, 200, 50);
}
