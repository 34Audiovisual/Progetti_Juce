/*
  ==============================================================================

    Meter.h
    Created: 2 Nov 2020 2:50:02pm
    Author:  Diego Capoccitti

  ==============================================================================
*/
#define NegInfDb -100.f
#define MaxDb 12.f

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

    
struct Meter : juce::Component
{
    Meter();
    ~Meter();
    void paint (juce::Graphics&) override;
    void resized() override;
    void update(float v);

    
private:
    float level;
    juce::ColourGradient cg;
    juce::Rectangle<int> bounds;
    int h, w, x, y;
    
public:

};
