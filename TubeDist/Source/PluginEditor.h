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
class TubeDistAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TubeDistAudioProcessorEditor (TubeDistAudioProcessor&);
    ~TubeDistAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider inGainSlider, outGainSlider, driveSlider, blendSlider;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TubeDistAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TubeDistAudioProcessorEditor)
};
