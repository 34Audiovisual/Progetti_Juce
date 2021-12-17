/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "SynthVoice.h"
#include "Tube.h"

//==============================================================================
/**
*/
class DualOscillatorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DualOscillatorAudioProcessor();
    ~DualOscillatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState apvts;
    juce::Synthesiser synth;
    float getFreq() {return currentFreq;};
    
    void updateOsc ()
    {
        for (int i = 0 ; i < synth.getNumVoices(); ++i)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                auto& output = *apvts.getRawParameterValue ("OUTPUT");
                auto& freq = *apvts.getRawParameterValue ("FREQ");
                auto& freqFine = *apvts.getRawParameterValue ("FREQFINE");
                auto& dirtyChoice = *apvts.getRawParameterValue ("DIRTY");
                auto& waveChoice = *apvts.getRawParameterValue ("WAVEFORM");
                voice->setGain(output.load());
                voice->setFreq(freq.load());
                voice->setFreqFine(freqFine.load());
                currentFreq = voice->getFreq();
                voice->setTube(dirtyChoice);
                voice->setWaveform(waveChoice);
            }
        }
    }
    

private:

    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    float currentFreq = 0.f;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DualOscillatorAudioProcessor)
};
