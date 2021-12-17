/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
 need to pass buffers to the Goniometer
 audio thread -> push buffer to Fifo continuosly
 GUI thread: check fifo for num available
    if numAvailable > 0
        copy the most recent buffer to buffer member var
        process member buffer accordingly.
*/

#include <array>

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

//struct LockingFifo
//{
//    LockingFifo();
//    ~LockingFifo();
//    void resizeBuffers(int chan, int sampleCount);
//    bool push(const AudioBuffer<float>& t);
//    bool pull(AudioBuffer<float>& t);
//private:
//    CriticalSection lock;
//    size_t readIndex{0}, writeIndex{0};
//    static constexpr size_t Capacity = 3;
//    std::array<AudioBuffer<float>, Capacity> buffers;
//    static void advanceIndex(size_t& index);
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LockingFifo)
//};
//
//struct LockFreeFifo
//{
//    using BufferType = juce::AudioBuffer<float>;
//
//    LockFreeFifo();
//    void resizeBuffers(int chan, int sampleCount);
//    void push(const BufferType& t);
//    void pull(BufferType& t);
//private:
//    juce::Atomic<size_t> readIndex{0}, writeIndex{0};
//    static constexpr size_t Capacity = 10;
//    std::array<BufferType, Capacity> buffers;
//    void advance();
//    juce::Atomic<bool> blocked { true };
//};
//
//struct ReadyBuffer
//{
//    ReadyBuffer() = default;
//    void resizeBuffers(int chan, int sampleCount);
//    bool push(const AudioBuffer<float>& t);
//    bool pull(AudioBuffer<float>& t);
//    bool isReady();
//private:
//    std::unique_ptr<LockingFifo> fifo;
//    Atomic<bool> readyState {false};
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReadyBuffer)
//};

namespace Identifiers
{
#define DECLARE_ID(name) const juce::Identifier name(#name);
    DECLARE_ID(project_10_settings)
    DECLARE_ID(enable_meterHold) //yes/no
    DECLARE_ID(hold_duration_ms) //holdTimeCB
    DECLARE_ID(decay_rate_db_sec) //decayCB
    DECLARE_ID(average_duration_ms) //averageCB
    DECLARE_ID(meter_display) //meterViewCB
    DECLARE_ID(goniometer_scale) //gonioScaleSlider
    DECLARE_ID(histogram_display) //histogramCB
    DECLARE_ID(rms_threshold)
    DECLARE_ID(mag_threshold)
};

class Pfmcpp_project10AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Pfmcpp_project10AudioProcessor();
    ~Pfmcpp_project10AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

//    std::vector<Atomic<float>> rmsLevels;
//    Atomic<float> rmsLevelsLeft, rmsLevelsRight;
//    Atomic<float> magnitudeLevelsLeft, magnitudeLevelsRight;
//    std::vector<Atomic<float>> magnitudeLevels;
    ValueSender<AudioBuffer<float>> bufferSender;
    int maxSamplesPerBlock = 0;
    double sampleRate = 44100.0;
    ValueSender<juce::AudioPlayHead::CurrentPositionInfo> positionInfoSender;

#define DO_SINE_DEBUG 0
#if DO_SINE_DEBUG
#define DEBUG_IN_PHASE 1
    dsp::Oscillator<float> osc;
#endif

    ValueTree settings { Identifiers::project_10_settings };
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessor)
};
