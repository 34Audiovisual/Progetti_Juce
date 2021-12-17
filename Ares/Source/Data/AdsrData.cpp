/*
  ==============================================================================

    AdsrData.cpp
    Created: 29 Oct 2021 9:15:03pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#include "AdsrData.h"

void AdsrData::updateADSR ( const float attack, const float decay, const float sustain, const float release)
{
    adsrParameters.attack = attack;
    adsrParameters.decay = decay;
    adsrParameters.sustain = sustain;
    adsrParameters.release = release;
    
    setParameters(adsrParameters);
}
