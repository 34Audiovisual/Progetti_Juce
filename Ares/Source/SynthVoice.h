/*
  ==============================================================================

    SynthVoice.h
    Created: 26 Oct 2021 12:31:18am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/AdsrData.h"
#include "SimpleFM.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    void update ( const float attack, const float decay, const float sustain, const float release);
    void updateFM ( float c, float m, float i) { pm.update(c, m, i);}
    void setGain (float g) { gain.setGainLinear(g);}
    void setCarrFreq ( float cf) { pm.setFreq(cf);}
    void setIndex ( float ix) { pm.setIndex(ix);}
    void setReverb ( bool r ) { reverb.setEnabled(r);}

private:
    SimpleFM pm;
    float carrFreq, modFreq, modIndex;
    AdsrData adsr;
    juce::AudioBuffer<float> synthBuffer;
    
    juce::dsp::Gain<float> gain;
    juce::dsp::Reverb reverb;
    
    bool isPrepared = false;
};
