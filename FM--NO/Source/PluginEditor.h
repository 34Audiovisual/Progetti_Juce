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
class FMAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FMAudioProcessorEditor (FMAudioProcessor&);
    ~FMAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider index, carr, mod;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FMAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMAudioProcessorEditor)
};
