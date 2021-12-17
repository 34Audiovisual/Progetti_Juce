/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class FMAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FMAudioProcessor();
    ~FMAudioProcessor() override;

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
    void setIndex (float i)
    {
        //idx = i;
         index = i;
    }
    void setCarrFreq(float cf)
    {
        carrFreq = cf;
        carrier.setFrequency(carrFreq);
    }
    void setModFreq(float mf)
    {
        modFreq = mf;
        modulator.setFrequency(modFreq);
    }
    

private:
    juce::dsp::Oscillator<float> carrier, modulator;
    juce::dsp::Gain<float> gain;
    float carrFreq = 273.0f;
    float modFreq = 207.0f;
    float index = 2.f;
    bool ph = true;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMAudioProcessor)
};
