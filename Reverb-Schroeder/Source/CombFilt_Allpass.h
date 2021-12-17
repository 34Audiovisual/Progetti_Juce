/*
  ==============================================================================

    ComboFilt_IIR.h
    Created: 13 Oct 2021 9:35:09pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CircularBuffer.h"

struct Comb_AllPass
{
    void prepare(float delayInMs, float decayInMs, double sampleRate)
    {
        gainFeedBack = gainCalc(delayInMs, decayInMs);
        delayInSample = static_cast<int>(delayInMs * sampleRate /1000.0);
        jassert(delayInSample <= 4410);
    }
    
    float gainCalc (float delayInMs, float decayInMs)
    {
        return std::pow (0.001, delayInMs / decayInMs);
    }
    
    void process (juce::AudioBuffer<float>& buffer)
    {
        auto bufferSize = buffer.getNumSamples();
        
        for (int sample = 0; sample < bufferSize; ++sample)
        {
            auto tBufferL = cBufferL.getBiggerBuffer();
            auto tBufferR = cBufferR.getBiggerBuffer();
            
            auto readIdxL = ( tBufferL.getNumSamples() + cBufferL.getReadIndex() - delayInSample) % tBufferL.getNumSamples();
            auto readIdxR = ( tBufferR.getNumSamples() + cBufferR.getReadIndex() - delayInSample) % tBufferR.getNumSamples();
            
            auto invBufferL = buffer.getSample(0, sample) * (-1.f * gainFeedBack);
            auto invBufferR = buffer.getSample(1, sample) * (-1.f * gainFeedBack);
            
            auto delayedBufferL = tBufferL.getSample(0, readIdxL) * gainFeedBack;
            auto delayedBufferR = tBufferR.getSample(0, readIdxR) * gainFeedBack;
            
            auto outBufferL = invBufferL + delayedBufferL;
            auto outBufferR = invBufferR + delayedBufferR;
            
            auto towriteBufferL = buffer.getSample(0, sample) + outBufferL;
            auto towriteBufferR = buffer.getSample(1, sample) + outBufferR;
            
            
            cBufferL.write(towriteBufferL);
            cBufferR.write(towriteBufferR);
        
            buffer.setSample(0, sample, outBufferL);
            buffer.setSample(1, sample, outBufferR);
        }
    }
    
private:
    int delayInSample = 10;
    float gainFeedBack = 1.0;
    
    float initValueCbuffer = 0.0f;
    int cBufferSizeInSample = 4410;
    CircularBuffer cBufferR {cBufferSizeInSample, initValueCbuffer}, cBufferL {cBufferSizeInSample, initValueCbuffer};
};
