/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Oscilloscope.h"


//==============================================================================
/**
*/
class OscillatorDSPAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    OscillatorDSPAudioProcessorEditor (OscillatorDSPAudioProcessor&);
    ~OscillatorDSPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override
    {
        auto buff = audioProcessor.getBuffer();
        auto* channelData = buff.getWritePointer (0);
        scope.pushBuffer(channelData, buff.getNumSamples());

    }

private:
    Oscilloscope scope;
    OscillatorDSPAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorDSPAudioProcessorEditor)
};
