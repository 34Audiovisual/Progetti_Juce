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
        
        levelSmooth.reset(sr, 0.1);
        freqSmooth.reset(sr, 0.1);
        phaseSmooth.reset(sr, 0.1);
        multSmooth.reset(sr, 0.1);
        levelGenSmooth.reset(sr, 0.1);
        offSetSmooth.reset(sr, 0.1);
    }
    
    void updateAngleDelta(float freq)
    {
        frequency = freq;
        auto cyclesPerSample = frequency / currentSampleRate;
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }
    
    float getFrequency () { return frequency; }
    float getLevel () { return level; }
    
    void changeLevel (float l)
    {
        //level = l;
        levelSmooth.setTargetValue(l);
    }
    
    void changeLevelGen (float l)
    {
        //level = l;
        levelGenSmooth.setTargetValue(l);
    }
    
    void changeMult (float m)
    {
        //mult = m;
        multSmooth.setTargetValue(m);
    }
    
    void changeFreq(float freq)
    {
        //updateAngleDelta(freq);
        freqSmooth.setTargetValue(freq);
    }
    
    void phaseMod (double mod)
    {
       // phase = mod ;//* juce::MathConstants<float>::twoPi;
        phaseSmooth.setTargetValue(mod);
    }
    
    void setOffset (double o)
    {
       // phase = mod ;//* juce::MathConstants<float>::twoPi;
        offSetSmooth.setTargetValue(o);
    }
    
    float nextFloat()
    {
        updateAngleDelta(freqSmooth.getNextValue());
        mult = multSmooth.getNextValue();
        phase = phaseSmooth.getNextValue();
        level = levelSmooth.getNextValue();
        levelGen = levelGenSmooth.getNextValue();
        offSet = offSetSmooth.getNextValue();
        
        auto currentSample = (float) std::sin (mult * currentAngle + phase);
        currentAngle += angleDelta;
        
        return currentSample * level * levelGen + offSet;
    }
    
    void renderSample (juce::AudioBuffer<float>& buffer, int sample)
    {
        updateAngleDelta(freqSmooth.getNextValue());
        mult = multSmooth.getNextValue();
        phase = phaseSmooth.getNextValue();
        level = levelSmooth.getNextValue();
        
        auto currentSample = (float) std::sin (mult * currentAngle + phase);
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
    float mult = 1.f;
    float offSet = 0.0f;
    float levelGen = 0.5;
    
    juce::SmoothedValue<float> levelSmooth {0.f};
    juce::SmoothedValue<float> levelGenSmooth {0.f};
    juce::SmoothedValue<float> freqSmooth {0.f};
    juce::SmoothedValue<float> phaseSmooth {0.f};
    juce::SmoothedValue<float> multSmooth {0.f};
    juce::SmoothedValue<float> offSetSmooth {0.f};
};
