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
class ReverbSchroederAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ReverbSchroederAudioProcessorEditor (ReverbSchroederAudioProcessor&);
    ~ReverbSchroederAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider decaySlider, dryWetSlider, earlySlider, lowFreqSlider, highFreqSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decaySliderAttachment, dryWetSliderAttachment, earlySliderAttachment, lowSliderAttachment, highSliderAttachment ;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReverbSchroederAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbSchroederAudioProcessorEditor)
};
