/*
  ==============================================================================

    SynthSound.h
    Created: 26 Oct 2021 12:31:40am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) override { return true;}
    bool appliesToChannel (int midiChannel) override { return true;}
    
    
};
