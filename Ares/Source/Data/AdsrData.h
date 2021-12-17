/*
  ==============================================================================

    AdsrData.h
    Created: 29 Oct 2021 9:15:03pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>

class AdsrData : public juce::ADSR
{
public:
    void updateADSR ( const float attack, const float decay, const float sustain, const float release);
private:
    juce::ADSR::Parameters adsrParameters;
    
};
