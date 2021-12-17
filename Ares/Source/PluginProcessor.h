/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Sequence.h"
#include "SynthSound.h"
#include "SynthVoice.h"

//==============================================================================
/**
*/
class AresAudioProcessor  : public juce::AudioProcessor, public  juce::AudioProcessorValueTreeState::Listener, public juce::Timer
{
public:
    float ftom(const float inputFreq) // converts frequency in Hz to midiPitch (as a float)
    {
        return 12.0f * log2(inputFreq / 440.f) + 69.0f;
    };
    
    void timerCallback () override
    {

        
        auto timeInterval = juce::Timer::getTimerInterval();


        if ((timeFromZero == 0 || timeFromZero > evTime * 1000) && onOff )
        {
            if (random)
            {
                auto newTime = 1 + rf.nextFloat() * (*apvts.getRawParameterValue("EVOLVE"));
                //float newTime = *apvts.getRawParameterValue("EVOLVE");
                setEvTime(newTime);
            }
            
            pos = r.nextInt(juce::Range<int>(0,14));
            auto dev = 1.02 - rf.nextFloat() * 0.04;
            DBG( "Position :" << pos << " - Time :"<< evTime);
            //float freqC = 55;
            float midiNote = ftom(freqC);
            
            
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice((int)numVoice)))
            {
                voice->updateFM(freqC * seq.takeNum(pos) * dev, freqC * seq.takeDenom(pos) * dev, indexMod);
            }
        
            
            
            midiBuf.clear();
            auto on = juce::MidiMessage::noteOn(1, midiNote, (juce::uint8) 100);
            //auto off = juce::MidiMessage::noteOff(1, midiNote, (juce::uint8) 100);
            auto off = juce::MidiMessage::allNotesOff(1);
                        
            midiBuf.addEvent(on, 100);
            juce::Timer::callAfterDelay( evTime * 1000, [off, this]{midiBuf.addEvent(off, 100);});
                        
            timeFromZero = 1;
            //pos += 1;
            numVoice = ! numVoice;
        }
 
        timeFromZero += timeInterval;
 
 
    }
    
    void setEvTime (float et)
    {
        evTime = et;
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->update(evTime, 0.1f, 1.f, evTime);
            }
        }
        
    }
    //==============================================================================
    AresAudioProcessor();
    ~AresAudioProcessor() override;

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
    float getFreq () { return freqC; }
    float getTime () { return evTime; }
    float getIndex () { return indexMod; }
    float getGain () { return gain; }
    bool getRandom () { return random; }
    
    juce::AudioProcessorValueTreeState apvts;


private:
    void parameterChanged(const juce::String& paramID, float newValue) override;
    juce::Synthesiser synth;
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    Sequence seq;
    juce::Random r, rf;

    int timeFromZero = 0;
    float freqC = 20.f;
    float indexMod = 1.f;
    bool onOff = false;
    bool random = false;
    float evTime = 5.f;
    float gain = 0.2;
    int pos = 0;
    juce::MidiBuffer midiBuf;
    bool numVoice = false;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AresAudioProcessor)
};
