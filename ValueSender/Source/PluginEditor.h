/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

const int NegativeInfinity = -66;
const int MaxDecibels = 12;

//==============================================================================
/**
*/
class ValueSenderAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    ValueSenderAudioProcessorEditor (ValueSenderAudioProcessor&);
    ~ValueSenderAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override
    {
        bool isPlaying = true; //assume we're playing.

        if( !audioProcessor.bufferSender.pull(buffer) )
        {
            DBG( "couldn't pull buffer from processor!" );
            if( !isPlaying )
            {
                DBG( "not playing, clearing internal buffer" );
                buffer.clear();
            }
            //else the last pull will get pushed again to all modules.
        }

        auto numSamples = buffer.getNumSamples();
        auto rmsLeftDb = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, numSamples),
                                                  float(NegativeInfinity));
        auto rmsRightDb = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, numSamples),
                                                   float(NegativeInfinity));
        auto magLeftDb = juce::Decibels::gainToDecibels(buffer.getMagnitude(0, 0, numSamples),
                                                  float(NegativeInfinity));
        auto magRightDb = juce::Decibels::gainToDecibels(buffer.getMagnitude(1, 0, numSamples),
                                                   float(NegativeInfinity));
    }

private:

    juce::AudioBuffer<float> buffer {2, 256};
    ValueSenderAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueSenderAudioProcessorEditor)
};
