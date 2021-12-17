/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CombFilt_IIR.h"
#include "CombFilt_Allpass.h"
#include "EarlyReflection.h"

//==============================================================================
/**
*/
class ReverbSchroederAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ReverbSchroederAudioProcessor();
    ~ReverbSchroederAudioProcessor() override;

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
    
    void setDryWet (float dw) { dryWet = dw;}
    void setEarly (float er) { early.setAmount(er);}
    void setDecay( float dec)
    {
        float reverbDecayInMs = dec;
        
        float delayComb1Ms = 29.7;
        comb1.prepare(delayComb1Ms, reverbDecayInMs, smRate);
        
        float delayComb2Ms = 37.1;
        comb2.prepare(delayComb2Ms, reverbDecayInMs, smRate);
        
        float delayComb3Ms = 41.1;
        comb3.prepare(delayComb3Ms, reverbDecayInMs, smRate);
        
        float delayComb4Ms = 43.7;
        comb4.prepare(delayComb4Ms, reverbDecayInMs, smRate);
    }
    void setLowPass(float freq) { lowpass.setCutoffFrequencyHz(freq); }
    void setHighPass(float freq) { highpass.setCutoffFrequencyHz(freq); }

    juce::AudioProcessorValueTreeState apvts;


private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParam ();
    
    Comb_IIR comb1, comb2, comb3, comb4;
    Comb_AllPass allpass1L, allpass2L, allpass1R, allpass2R;
    EarlyReflection early;
    float dryWet = 0.2;
    juce::dsp::LadderFilter<float> lowpass;
    juce::dsp::LadderFilter<float> highpass;
    double smRate = 44100;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbSchroederAudioProcessor)
};
