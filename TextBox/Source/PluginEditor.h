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
class TextBoxAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TextBoxAudioProcessorEditor (TextBoxAudioProcessor&);
    ~TextBoxAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextEditor text;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TextBoxAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextBoxAudioProcessorEditor)
};
