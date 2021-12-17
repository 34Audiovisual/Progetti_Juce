/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "dRowAudio_Clock.h"

//==============================================================================
/**
*/
class ClockAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ClockAudioProcessorEditor (ClockAudioProcessor&);
    ~ClockAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Clock clock;
    ClockAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClockAudioProcessorEditor)
};
