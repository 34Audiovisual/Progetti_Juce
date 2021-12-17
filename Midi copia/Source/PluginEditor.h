/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MidiAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MidiAudioProcessorEditor (MidiAudioProcessor&);
    ~MidiAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiAudioProcessor& audioProcessor;
    juce::TextButton messMidi;
    juce::String scritta {"messaggio "};
    juce::String& messaggioMidi;
    int i = 0;

    juce::String messaggio {"Messaggio Midi"};
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiAudioProcessorEditor)
};
