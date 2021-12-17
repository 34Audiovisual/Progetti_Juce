/*
  ==============================================================================

    EarlyReflection.h
    Created: 18 Oct 2021 4:45:46pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "CircularBuffer.h"

struct EarlyReflection
{
    void prepare(double sRate)
    {
        sampleRate = sRate;
        setDelays(19.9f, 35.4f, 38.9f, 41.4f, 69.9f, 79.6f);
        setGains(1.02f, 0.818f, 0.635f, 0.719f, 0.267f, 0.242f);
    }
    
    void setDelays ( float t1, float t2, float t3, float t4, float t5, float t6)
    {
        delaysInSm1 = msToSample(t1);
        delaysInSm2 = msToSample(t2);
        delaysInSm3 = msToSample(t3);
        delaysInSm4 = msToSample(t4);
        delaysInSm5 = msToSample(t5);
        delaysInSm6 = msToSample(t6);
    }
    
    int msToSample(float ms)
    {
        float sample = static_cast<int>(( ms/1000.f ) * sampleRate);
        return sample;
    }
    
    void setGains ( float g1, float g2, float g3, float g4, float g5, float g6)
    {
        gain1 = g1;
        gain2 = g2;
        gain3 = g3;
        gain4 = g4;
        gain5 = g5;
        gain6 = g6;
    }
    
    juce::AudioBuffer<float> process (juce::AudioBuffer<float>& buffer)
    {
        auto bufferSize = buffer.getNumSamples();
        juce::AudioBuffer<float> tempBuff;
        tempBuff.makeCopyOf(buffer);
    
        for (int sample = 0; sample < bufferSize; ++sample)
        {
            auto tBufferL = cBufferL.getBiggerBuffer();
            auto tBufferR = cBufferR.getBiggerBuffer();
            
            auto readIdx1 = ( tBufferL.getNumSamples() + cBufferL.getReadIndex() - delaysInSm1) % tBufferL.getNumSamples();
            auto readIdx2 = ( tBufferL.getNumSamples() + cBufferL.getReadIndex() - delaysInSm2) % tBufferL.getNumSamples();
            auto readIdx3 = ( tBufferL.getNumSamples() + cBufferL.getReadIndex() - delaysInSm3) % tBufferL.getNumSamples();
            auto readIdx4 = ( tBufferL.getNumSamples() + cBufferL.getReadIndex() - delaysInSm4) % tBufferL.getNumSamples();
            auto readIdx5 = ( tBufferL.getNumSamples() + cBufferL.getReadIndex() - delaysInSm5) % tBufferL.getNumSamples();
            auto readIdx6 = ( tBufferL.getNumSamples() + cBufferL.getReadIndex() - delaysInSm6) % tBufferL.getNumSamples();
            
            auto delayed1 = tBufferL.getSample(0, readIdx1) * gain1;
            auto delayed2 = tBufferR.getSample(0, readIdx2) * gain2;
            auto delayed3 = tBufferR.getSample(0, readIdx3) * gain3;
            auto delayed4 = tBufferL.getSample(0, readIdx4) * gain4;
            auto delayed5 = tBufferL.getSample(0, readIdx5) * gain5;
            auto delayed6 = tBufferR.getSample(0, readIdx6) * gain6;
            
            auto towriteBufferL = buffer.getSample(0, sample);
            auto towriteBufferR = buffer.getSample(1, sample);
            
            auto outBufferL = delayed1 + delayed4 + delayed5;
            auto outBufferR = delayed2 + delayed3 + delayed6;
            
            cBufferL.write(towriteBufferL);
            cBufferR.write(towriteBufferR);
            
            tempBuff.setSample(0, sample, outBufferL * amount);
            tempBuff.setSample(1, sample, outBufferR * amount);
        }
        return tempBuff;
    }
    
    void setAmount (float am)
    {
        amount = am;
    }
    
private:
    int delaysInSm1, delaysInSm2, delaysInSm3, delaysInSm4, delaysInSm5, delaysInSm6;
    float gain1, gain2, gain3, gain4, gain5, gain6;
    double sampleRate = 44100;
    float amount = 1;
        
        
    float initValueCbuffer = 0.0f;
    int cBufferSizeInSample = 4410;
    CircularBuffer cBufferR {cBufferSizeInSample, initValueCbuffer}, cBufferL {cBufferSizeInSample, initValueCbuffer};
};
