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
    //osc.setFrequency (juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));;
    //adsr.noteOn();
}
void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
   // adsr.noteOff();
    
   // if (! allowTailOff || ! adsr.isActive())
    //    clearCurrentNote();
}
void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
     
    osc.prepare (spec);
    osc.setFrequency(440);

    gain.prepare (spec);
     
    gain.setGainLinear (0.9f);
    
    //TUBE INIT
    tube.setSampleRate(sampleRate);
    setTube(0);
    //END TUBE INIT
    
    isPrepared = true;
}
void SynthVoice::update ( const float attack, const float decay, const float sustain, const float release)
{
    adsr.updateADSR(attack, decay, sustain, release);
}
void SynthVoice::renderNextBlock (juce::AudioBuffer<float> &outputBuffer, int startSample, int numSample)
{
    jassert(isPrepared);
    
    //if (! isVoiceActive())
     //   return;
    
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSample, false,false, true);

    synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
    osc.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    //TUBE
    for (int sample = 0; sample < synthBuffer.getNumSamples(); ++sample)
    {
        auto currentSampleL = synthBuffer.getSample(0, sample);
        auto currentSampleR = synthBuffer.getSample(1, sample);
        synthBuffer.setSample(0, sample, tube.perform(currentSampleL));
        synthBuffer.setSample(1, sample, tube.perform(currentSampleR));
    }
    //TUBE
    
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
