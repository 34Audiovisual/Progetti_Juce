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
class AllPassbiquadAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AllPassbiquadAudioProcessorEditor (AllPassbiquadAudioProcessor&);
    ~AllPassbiquadAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider frequency;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AllPassbiquadAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllPassbiquadAudioProcessorEditor)
};
