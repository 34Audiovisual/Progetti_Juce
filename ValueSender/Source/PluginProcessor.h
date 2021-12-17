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
//this comes from @quicknir (Nir Friedman) in the cpplang slack
template<class T>
struct ValueSender
{
    void prepare(int numSamples, int numChannels)
    {
        if constexpr (std::is_same<T, juce::AudioBuffer<float>>::value )
        {
            for( auto& buf : buffer )
                buf.setSize(numChannels, numSamples);
        }
    }
                          
    void push(const T& f)
    {
        //        std::cout << "writing to idx:" << writer.index << "\n";
        buffer[writer.index] = f; //1) [{1,false}]
        writer.fresh = true;        //1) [{1,true}]

        writer = reserve.exchange(writer); //switches 'writer' with whatever was in 'reserved'
        //1) [writer: {2,false}.  reserve:{1,true}]
    }

    bool pull(T& t)
    {
        reader.fresh = false; //{0,false}
        reader = reserve.exchange(reader);  //switches 'reader' with whatever was in 'reserve'
        //reader: {1,true} reserve: {0,false}

        if (!reader.fresh)
        {
            //            std::cout << "reader isn't fresh!\n";
            return false;
        }

        //std::cout << "reading from idx:" << reader.index << "\n";
        t = buffer[reader.index];
        return true;
    }
private:
    std::array<T, 3> buffer;
    struct DataIndex
    {
        int index;
        bool fresh = false;
    };

    DataIndex reader = {0};
    DataIndex writer = {1};
    /*
     the reserve always atomically holds the last reader or writer.
     */
    std::atomic<DataIndex> reserve { DataIndex{2} };
};


class ValueSenderAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ValueSenderAudioProcessor();
    ~ValueSenderAudioProcessor() override;

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
    
    int maxSamplesPerBlock = 0;
    double sampleRate = 44100.0;
    ValueSender<juce::AudioBuffer<float>> bufferSender;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueSenderAudioProcessor)
};
