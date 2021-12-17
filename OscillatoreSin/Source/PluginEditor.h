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
class OscillatoreSinAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    OscillatoreSinAudioProcessorEditor (OscillatoreSinAudioProcessor&);
    ~OscillatoreSinAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider freqSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqSliderAttachment;
    
    OscillatoreSinAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatoreSinAudioProcessorEditor)
};
