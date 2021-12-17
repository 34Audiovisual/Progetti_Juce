/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TextBoxAudioProcessorEditor::TextBoxAudioProcessorEditor (TextBoxAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(text);
    text.setCaretVisible(false);
    text.onTextChange = [this]{repaint();};
    text.setInputRestrictions(5,"abcd0123456789");
    setSize (400, 300);
}

TextBoxAudioProcessorEditor::~TextBoxAudioProcessorEditor()
{
}

//==============================================================================
void TextBoxAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    juce::String testo = text.getText();
    auto risultato = testo.getIntValue() + 7;
    //juce::String testo = "cazzo";
    g.drawFittedText (juce::String(risultato), getLocalBounds(), juce::Justification::centred, 1);
}

void TextBoxAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    text.setBounds(10, 10, 150, 50);
}
