/*
  ==============================================================================

    Delay.h
    Created: 27 Sep 2021 2:19:59am
    Author:  Diego Capoccitti

  ==============================================================================
*/
#include <JuceHeader.h>
#pragma once

class Delay
{
public:
    void getFromBiggerBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& biggerBuffer, int writePosition, int channel);
    void prepare(double sr, int dt)
    {
        sampleRate = sr;
        delayTimeInMs = dt;
    }
    
    void setDelayTime (float dt)
    {
        delayTimeInMs = dt;
    }
    
    void setDelayGain (float dg)
    {
        gainDelay = dg;
    }
    
    void addBuffer (juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& bufferToAdd, int ch, int destStartSample, int sourceStartSample, int numSamplesToAdd)
    {
        for (int sample = 0; sample < numSamplesToAdd; ++sample)
        {
            auto sampleSum = (buffer.getSample(ch, (destStartSample + sample)) + bufferToAdd.getSample(ch, (sourceStartSample + sample))* gainDelay);
            buffer.setSample(ch, (destStartSample + sample), sampleSum);
        }
    }
    
    
private:
    float delayTimeInMs = 1000;
    double sampleRate = 44100.0;
    float gainDelay = 1.0;
};
