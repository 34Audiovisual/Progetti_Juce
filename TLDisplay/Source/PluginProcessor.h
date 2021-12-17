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
class ControlerMidiAudioProcessor  : public juce::AudioProcessor, juce::Timer
{
public:
    //==============================================================================
    ControlerMidiAudioProcessor();
    ~ControlerMidiAudioProcessor() override;
    
    void timerCallback() override
    {
        
    }

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
    
    //il midi channel deve essere tra 0-15 !!!!!!!!!
    void addEvent (int d, int p, std::array<int, 8> array1, std::array<int, 8> array2)
    {

        midiCCmess = makeCCmess(d, p);
        midiNotes1 = makeNotesMess(array1, d);
        midiNotes2 = makeNotesMess(array2, d);
        
        if (midiDev != nullptr)
        {
            midiDev->sendMessageNow(midiCCmess);
        
            for (int i = 0; i < midiNotes1.size() ; ++i)
            {
                midiDev->sendMessageNow(midiNotes1[i]);
            }
        
            for (int i = 0; i < midiNotes2.size() ; ++i)
            {
                midiDev->sendMessageNow(midiNotes2[i]);
            }
        }
    }

    juce::MidiMessage makeCCmess (int d, int p)
    {
        juce::MidiMessage m;
        m = juce::MidiMessage::programChange(d, p);
        
        return m;
    }
    
    std::array<juce::MidiMessage, 8> makeNotesMess (    std::array<int, 8> array, int d)
    {
        std::array<juce::MidiMessage, 8> messArray;
        
        for (int i = 0; i < messArray.size() ; ++i)
        {
            auto m = juce::MidiMessage::noteOn(d, array[i], (juce::uint8) 100);
            messArray[i] = m;
        }
        
        return messArray;
    }
    
    std::array<int, 8> makeNoteArray (juce::String string)
    {
        std::array<int, 8> array;
        
        for (int i = 0; i < string.length(); ++i)
            array[i] = static_cast<int>(string[i]);
        
        return array;
    }
    
    void retrig();
    
    juce::ValueTree valuesTreeMidi {"VALUE"};
    
private:
    juce::MidiMessage midiCCmess;
    std::array<juce::MidiMessage, 8> midiNotes1, midiNotes2;
    
    juce::String currentMidiMess;
    std::unique_ptr<juce::MidiOutput> midiDev = nullptr;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlerMidiAudioProcessor)
};
