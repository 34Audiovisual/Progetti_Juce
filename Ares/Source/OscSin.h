/*
  ==============================================================================

    OscSin.h
    Created: 10 Nov 2021 3:20:15pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct OscSin
{
    void prepare (double sr, float f)
    {
        currentSampleRate = sr;
        updateAngleDelta(f);
    }
    
    void updateAngleDelta(float freq)
    {
        frequency = freq;
        auto cyclesPerSample = frequency / currentSampleRate;
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }
    
    void phaseMod (double mod)
    {
        phase = mod ;//* juce::MathConstants<float>::twoPi;
    }
    
    float getFreqValue()
    {

        float f = 0;
        return f;

    }
    
    void changeFreq(float freq)
    {
        updateAngleDelta(freq);
    }
    
    float getFrequency () { return frequency; }
    
    float nextFloat()
    {
        auto currentSample = (float) std::sin (currentAngle + phase);
        currentAngle += angleDelta;
        
        return currentSample;
    }
    
    void renderSample (juce::AudioBuffer<float>& buffer, int sample)
    {
        auto currentSample = (float) std::sin (currentAngle + phase);
        currentAngle += angleDelta;
        
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* dataBuffer  = buffer.getWritePointer (channel);
            dataBuffer[sample]  = currentSample * level;
        }
    }
    
private:
    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
    
    float level = 0.9f;
    float frequency = 0;
    float phase = 0;
};
