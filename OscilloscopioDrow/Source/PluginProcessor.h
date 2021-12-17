/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Oscillator.h"

//==============================================================================
/**
*/
class OscillatorDSPAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    OscillatorDSPAudioProcessor();
    ~OscillatorDSPAudioProcessor() override;

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

    juce::AudioBuffer<float>& getBuffer () { return bufferExt; }
    
private:
    Oscillator osc;
    juce::dsp::Reverb rev;
    juce::Reverb::Parameters theReverbParameters;
    juce::dsp::DelayLine<float> delay {88200};
    juce::dsp::FirstOrderTPTFilter<float> allpass;
    juce::dsp::LadderFilter<float> lowpass;
    
    juce::dsp::IIR::Filter<float> filter;
    juce::dsp::IIR::Coefficients<float> coeff;
    
    juce::AudioBuffer<float> bufferExt;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorDSPAudioProcessor)
};
