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

struct SimplePM
{
    void prepare (double sampleRate, float carrFreq, float modFreq, float modIndex)
    {
        carrierFreq = carrFreq;
        modulatorFreq = modFreq;
        carrier.prepare(sampleRate, carrierFreq);
        modulator.prepare(sampleRate, modulatorFreq);
        index = modIndex;
    }
    
    void renderBlock (juce::AudioBuffer<float>& buffer)
    {
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            /*
            auto dev = index * modulator.getFrequency();
            auto currentDev = dev * modulator.nextFloat();
            auto currentFreq = carrierFreq + currentDev;
            carrier.changeFreq(currentFreq);
            carrier.renderSample(buffer, sample);
             */
            
            auto dev = index / juce::MathConstants<float>::twoPi;
            auto currentDev = dev * modulator.nextFloat();
            carrier.phaseMod(currentDev);
            carrier.renderSample(buffer, sample);
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
    
private:
    OscSin carrier, modulator;
    float index = 0.f;
    float carrierFreq, modulatorFreq;
};


