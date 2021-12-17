/*
  ==============================================================================

    Delay.h
    Created: 27 Sep 2021 2:19:59am
    Author:  Diego Capoccitti

  ==============================================================================
*/
#include <JuceHeader.h>
#pragma once

class Flanger
{
public:
    
    void getFromBiggerBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& biggerBuffer, int writePosition, int channel);
    void prepare(double sr, int dt)
    {
        sampleRate = sr;
        delayTimeInMs = dt;
        currentSampleRate = sampleRate;
        updateAngleDelta();
    }
    
    void setDelayTime (float dt)
    {
        delayTimeInMs = dt;
    }
    
    void updateAngleDelta()
        {
            auto cyclesPerSample = speed / currentSampleRate;
            angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
        }
    
    
private:
    float delayTimeInMs = 1000;
    double sampleRate = 44100.0;
    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
    float widthInMs = 20;
    float delayMinInMs = 3;
    float speed = 5;
};
