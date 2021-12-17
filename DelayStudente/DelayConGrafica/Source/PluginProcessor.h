/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginParameters.h"
#include "Delay.h"
#include "CircularBuffer.h"

//==============================================================================
/**
*/
class DelayAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    DelayAudioProcessor();
    ~DelayAudioProcessor() override;

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
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

        params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_DW, "Dry/Wet", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f), DEFAULT_DW));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_DELAY_TIME, "Delay time", juce::NormalisableRange<float>(0.0f, MAX_DELAY_TIME, 1.0f, 0.5f), DEFAULT_DELAY_TIME));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_DELAY_TIME_L, "Delay time L", juce::NormalisableRange<float>(0.0f, MAX_DELAY_TIME, 1.0f, 0.5f), DEFAULT_DELAY_TIME_L));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_DELAY_TIME_R, "Delay time R", juce::NormalisableRange<float>(0.0f, MAX_DELAY_TIME, 1.0f, 0.5f), DEFAULT_DELAY_TIME_R));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_FB, "Feedback", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f), DEFAULT_FB));
        params.push_back(std::make_unique<juce::AudioParameterBool>(NAME_CC, "Channel Configuration", DEFAULT_CC));

        return { params.begin(), params.end() };
    }
    
    bool CC = DEFAULT_CC;
    
    juce::AudioProcessorValueTreeState parameters;

private:
    void parameterChanged(const juce::String& paramID, float newValue) override;



    CircularBuffer circularBuffer;
    Delay delay;
    Delay delayL;
    Delay delayR;

    float delayT = 0.0f;
    float delayTL = 0.0f;
    float delayTR = 0.0f;
    float feedb = 0.0f;

    float dryWet = 0.0f;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessor)
};
