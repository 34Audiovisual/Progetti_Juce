/*
  ==============================================================================

    SimpleFM.h
    Created: 10 Nov 2021 3:52:51pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "OscSin.h"

struct SimpleFM
{
    enum Mode
    {
        FM = 1,
        PM
    };
    
    void prepare (double sampleRate, float carrFreq, float modFreq, float modIndex, Mode m = PM)
    {
        carrierFreq = carrFreq;
        modulatorFreq = modFreq;
        carrier.prepare(sampleRate, carrierFreq);
        modulator.prepare(sampleRate, modulatorFreq);
        index = modIndex;
        mode = m;
        
        indexSmooth.reset(sampleRate, 0.5);
    }
    
    void renderBlock (juce::AudioBuffer<float>& buffer)
    {
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            index = indexSmooth.getNextValue();
            
            switch (mode)
            {
                case FM:
                {
                    auto dev = index * modulatorFreq;
                    auto currentDev = dev * modulator.nextFloat();
                    auto currentFreq = carrierFreq + currentDev;
                    carrier.changeFreq(currentFreq);
                    carrier.renderSample(buffer, sample);
                }
                    break;
                    
                case PM:
                {
                    //auto dev = index / juce::MathConstants<float>::twoPi;
                    auto currentDev = static_cast<double>(index * modulator.nextFloat());
                    carrier.phaseMod(currentDev);
                    carrier.renderSample(buffer, sample);
                }
                    break;
            }
            
        }
    }
    
    void update(float carrFreq, float modFreq, float ind)
    {
        index = ind;
        carrierFreq = carrFreq;
        modulatorFreq = modFreq;
        carrier.changeFreq(carrFreq);
        modulator.changeFreq(modFreq);
    }
    
    void setFreq ( float freq)
    {
        carrierFreq = freq;
        carrier.changeFreq(freq);
    }
    
    void setIndex ( float i)
    {
        index = i;
        indexSmooth.setTargetValue(index);
    }
    
private:
    OscSin carrier, modulator;
    float index = 0.f;
    float carrierFreq, modulatorFreq;
    Mode mode{FM};
    
    juce::SmoothedValue<float> indexSmooth {0.f};
    
};


