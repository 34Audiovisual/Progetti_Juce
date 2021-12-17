/*
  ==============================================================================

    ComboFilt_IIR.h
    Created: 13 Oct 2021 9:35:09pm
    Author:  Diego Capoccitti

  ==============================================================================
 
 Dodge e Jerse (1997) suggeriscono che per calcolare il fattore di moltiplicazione utile per ottenere un tempo di decay con un abbassamento di 60 dB in un filtro Comb IIR, la formula è la seguente:
 
 g = 0.001^(D/T)
 
 dove:
 
 g : fattore di moltiplicazione (qui chiamato gainFeedback)
 D : delay time (qui chiamato delayInSample, ma in sec non in sample)
 T : tempo di decay in sec
 
 quindi per esempio se D è di 50 ms, e vogliamo ottenere 2 sec di decay, allora g = 0.001^(0,05/2) = 0.841395
 
 L'inverso di D invece fornisce la frequenza dell'altezza della risonanza del filtro (utile per costruire dei filtri risonanti)
 
 by
 
 Diego Capoccitti (Ottobre 2021)
*/

#pragma once
#include <JuceHeader.h>
#include "CircularBuffer.h"

struct Comb_IIR
{
    void prepare(float delayInMs, float decayInMs, double sampleRate)
    {
        gainFeedBack = gainCalc(delayInMs, decayInMs);
        delayInSample = static_cast<int>(delayInMs * sampleRate /1000.0);
        jassert(delayInSample <= 4410);
    }
    
    float gainCalc (float delayInMs, float decayInMs)
    {
        DBG("Gain : " << std::pow (0.001, delayInMs / decayInMs));
        
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
            
            auto delayedBufferL = tBufferL.getSample(0, readIdxL) * gainFeedBack;
            auto delayedBufferR = tBufferR.getSample(0, readIdxR) * gainFeedBack;
            
            
            auto towriteBufferL = buffer.getSample(0, sample) + delayedBufferL * gainFeedBack;
            auto towriteBufferR = buffer.getSample(1, sample) + delayedBufferR * gainFeedBack;
            
            auto outBufferL = towriteBufferL;
            auto outBufferR = towriteBufferR;
            
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
