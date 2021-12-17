/*
  ==============================================================================

    Oscillator.h
    Created: 16 Oct 2021 10:21:27am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct Oscillator
{
    Oscillator()
    {
        oscil.initialise([] (float x) { return std::sin (x); }, 128);
    }
    
    ~Oscillator()
    {
        
    }
    
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        oscil.prepare(spec);
    }
    
template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        oscil.process(context);
    }
    
    void reset() noexcept
    {
        oscil.reset();
    }
    
    void setFreq (int f)
    {
        oscil.setFrequency(f);
    }

private:
    juce::dsp::Oscillator<float> oscil;
};
