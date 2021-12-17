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
class OscillatoreSinAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    OscillatoreSinAudioProcessor();
    ~OscillatoreSinAudioProcessor() override;

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
    
    void updateAngleDelta()
    {
        auto cyclesPerSample = getFreqValue() / currentSampleRate;
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParam();
    
    float getFreqValue()
    {
        auto f = apvts.getRawParameterValue("FREQ");
       /// DBG(f->load());
        return f->load();

    }
    
    void changeFreq()
    {
        if (currentSampleRate > 0.0)
            updateAngleDelta();
    }

private:
    double currentSampleRate = 0.0, currentAngle = 0.0, angleDelta = 0.0;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatoreSinAudioProcessor)
};
