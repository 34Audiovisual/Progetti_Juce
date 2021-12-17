/*
  ==============================================================================

    SynthVoice.cpp
    Created: 26 Oct 2021 12:31:18am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound * sound, int currentPitchWheelPosition)
{
    pm.setFreq(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    
    adsr.noteOn();
}
void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    
    if (! allowTailOff || ! adsr.isActive())
        clearCurrentNote();
}
void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    carrFreq = 200.f;
    modFreq = 201.f;
    modIndex = 1.0f;
    pm.prepare(sampleRate, carrFreq, modFreq, modIndex);
    
    adsr.setSampleRate(sampleRate);
    float a = 5.0f;
    float d = 0.1f;
    float s = 1.0f;
    float r = 5.0f;
    adsr.updateADSR(a, d, s, r);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
     
    gain.prepare (spec);
    gain.setRampDurationSeconds(0.5);
    gain.setGainLinear (0.2f);
    
    reverb.prepare(spec);
    juce::dsp::Reverb::Parameters revParam;
    revParam.dryLevel = 0.1;
    revParam.width = 1.0;
    revParam.damping = 0.1;
    revParam.roomSize = 1.0;
    revParam.wetLevel = 0.2;
    reverb.setParameters(revParam);
    
    isPrepared = true;
}
void SynthVoice::update ( const float attack, const float decay, const float sustain, const float release)
{
    adsr.updateADSR(attack, decay, sustain, release);
}
void SynthVoice::renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSample)
{
    jassert(isPrepared);
    
    if (! isVoiceActive())
        return;
    
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSample, false,false, true);

    synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
    
    pm.renderBlock(synthBuffer);

    reverb.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    
    


    
    for (int channel = 0; channel <outputBuffer.getNumChannels(); ++ channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSample);
    
        if (! adsr.isActive())
        {
            clearCurrentNote();
        }
    }
}
