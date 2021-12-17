/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Granulator.h"
#include "PluginParameters.h"

//==============================================================================
/**
*/
class GranulatorAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    GranulatorAudioProcessor();
    ~GranulatorAudioProcessor() override;

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
    void makePluginMono ();
    
    void parameterChanged(const juce::String& paramID, float newValue) override;
    
    juce::AudioProcessorValueTreeState parameters;
    juce::ValueTree setting { "File" };

    juce::AudioBuffer<float> loadedFile {1, 44100};
    void prepareGranulator ();

    GranulatorVoices& getGranulatorL() {return granulatorsL;}
    GranulatorVoices& getGranulatorR() {return granulatorsR;}
    int getReadIdx () { return readIdx; }
    int getNumChannel() { return getTotalNumOutputChannels(); }
    bool isPlaying = false;
    bool mono = false;
    bool link = false;
    bool isPluginMono = false;
    
private:
    std::size_t nVoices = 10;
    GranulatorVoices granulatorsL { loadedFile, nVoices };
    GranulatorVoices granulatorsR { loadedFile, nVoices };
    bool isChangingVoices {false};
    int readIdx = 0;
    float linkedDiff = 0;
    float spread = 0.5f;

    
    juce::String path {"null"};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranulatorAudioProcessor)
};
