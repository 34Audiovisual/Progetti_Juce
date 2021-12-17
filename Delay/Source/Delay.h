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
    
    
private:
    float delayTimeInMs = 1000;
    double sampleRate = 44100.0;
    float gainDelay = 1.0;
};
