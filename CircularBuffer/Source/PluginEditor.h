/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Meter.h"

//==============================================================================
/**
*/
class CircularBufferAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    CircularBufferAudioProcessorEditor (CircularBufferAudioProcessor&);
    ~CircularBufferAudioProcessorEditor() override;

    //==============================================================================
    
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override
    {
        auto magn = audioProcessor.getMagnCircular();
        auto v = juce::Decibels::gainToDecibels(magn);
        meter.update(v);
        //DBG(audioProcessor.getMagnCircular());
    }

private:

    CircularBufferAudioProcessor& audioProcessor;
    Meter meter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircularBufferAudioProcessorEditor)
};
