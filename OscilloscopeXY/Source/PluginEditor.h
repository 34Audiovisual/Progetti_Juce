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
class OscilloscopeXYAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    OscilloscopeXYAudioProcessorEditor (OscilloscopeXYAudioProcessor&);
    ~OscilloscopeXYAudioProcessorEditor() override;
    
    void timerCallback() override
    {
        auto buff = audioProcessor.getBuffer();
        
        scope.pushBuffer(buff);

    }

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Oscilloscope scope;
    
    OscilloscopeXYAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscilloscopeXYAudioProcessorEditor)
};
