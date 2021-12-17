/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
//LockingFifo::LockingFifo()
//{
//    resizeBuffers(2, 512);
//}
//
//LockingFifo::~LockingFifo()
//{
//    ScopedTryLock stl(lock);
//    while( !stl.isLocked() )
//    {
//        //wait for a push/pull/resize to finish before finishing destruction.
//        DBG( "LockingFifo::~LockingFifo() waiting for push/pull/resize to finish before completing destruction" );
//    }
//}
//
//void LockingFifo::resizeBuffers(int chan, int sampleCount)
//{
//    const ScopedLock sl(lock);
//    AudioBuffer<float> buf(chan, sampleCount);
//    buf.clear();
//
//    for( auto& b : buffers )
//    {
//        b.makeCopyOf(buf);
//    }
//}
//
//bool LockingFifo::push(const AudioBuffer<float> &t)
//{
//    ScopedTryLock stl(lock);
//    if( stl.isLocked() )
//    {
//        buffers.at(writeIndex).makeCopyOf(t);
//        advanceIndex(writeIndex);
//        return true;
//    }
//    return false;
//}
//
//bool LockingFifo::pull(AudioBuffer<float> &t)
//{
//    ScopedTryLock stl(lock);
//    if( stl.isLocked() )
//    {
//        t.makeCopyOf( buffers[readIndex] );
//        advanceIndex(readIndex);
//        return true;
//    }
//    return false;
//}
//
//void LockingFifo::advanceIndex(size_t &index)
//{
//    ++index;
//    if( index > Capacity - 1)
//        index = 0;
//}
//==============================================================================
//LockFreeFifo::LockFreeFifo() { resizeBuffers(2, 256); }
//
//void LockFreeFifo::resizeBuffers(int chan, int sampleCount)
//{
//    blocked.set(true);
//
//    writeIndex.set(0);
//    readIndex.set(0);
//
//    AudioBuffer<float> buf(chan, sampleCount);
//    buf.clear();
//
//    for( auto& b : buffers )
//        b.makeCopyOf(buf);
//
//    blocked.set(false);
//}
//
//void LockFreeFifo::push(const BufferType& t)
//{
//    blocked.set(true);
//
//    buffers.at(writeIndex.get()).makeCopyOf(t);
//    advance();
//
//    blocked.set(false);
//}
//
//void LockFreeFifo::pull(BufferType& t)
//{
//    if( blocked.get() )
//        return;
//
//    t.makeCopyOf( buffers.at(readIndex.get()) );
//}
//
//void LockFreeFifo::advance()
//{
//    auto write = writeIndex.get();
//    ++write;
//    if( write > Capacity - 1 )
//        write = 0;
//
//    readIndex.exchange( writeIndex.exchange(write) );
//}
//==============================================================================
//void ReadyBuffer::resizeBuffers(int chan, int sampleCount)
//{
//    readyState.set(false);
//    auto swapFifo = std::make_unique<LockingFifo>();
//    swapFifo->resizeBuffers(chan, sampleCount);
//    fifo.swap(swapFifo);
//    readyState.set(true);
//}
//bool ReadyBuffer::push(const AudioBuffer<float>& t)
//{
//    if( isReady() )
//    {
//        return fifo->push(t);
//    }
//    return false;
//}
//bool ReadyBuffer::pull(AudioBuffer<float>& t)
//{
//    if( isReady() )
//    {
//        return fifo->pull(t);
//    }
//    return false;
//}
//bool ReadyBuffer::isReady()
//{
//    return readyState.get() && fifo.get() != nullptr;
//}
//==============================================================================
Pfmcpp_project10AudioProcessor::Pfmcpp_project10AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    settings.setProperty(Identifiers::enable_meterHold,
                         true,
                         nullptr);

    settings.setProperty(Identifiers::hold_duration_ms,
                         2, // 0.5s
                         nullptr);

    settings.setProperty(Identifiers::decay_rate_db_sec,
                         4, //-24db/s
                         nullptr);

    settings.setProperty(Identifiers::average_duration_ms,
                         4, //1000ms
                         nullptr);

    settings.setProperty(Identifiers::meter_display,
                         1, //both
                         nullptr);

    settings.setProperty(Identifiers::goniometer_scale,
                         100, //50% - 200%
                         nullptr);

    settings.setProperty(Identifiers::histogram_display,
                         1, //stacked
                         nullptr);

    settings.setProperty(Identifiers::rms_threshold,
                         0, //0dbFS
                         nullptr);

    settings.setProperty(Identifiers::mag_threshold,
                         0, //0dbFS
                         nullptr);
}

Pfmcpp_project10AudioProcessor::~Pfmcpp_project10AudioProcessor()
{
}

//==============================================================================
const String Pfmcpp_project10AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Pfmcpp_project10AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Pfmcpp_project10AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Pfmcpp_project10AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Pfmcpp_project10AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Pfmcpp_project10AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Pfmcpp_project10AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Pfmcpp_project10AudioProcessor::setCurrentProgram (int index)
{
}

const String Pfmcpp_project10AudioProcessor::getProgramName (int index)
{
    return {};
}

void Pfmcpp_project10AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Pfmcpp_project10AudioProcessor::prepareToPlay (double sr, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    sampleRate = sr;
    auto layout = getBusesLayout();

    maxSamplesPerBlock = samplesPerBlock;

    AudioBuffer<float> buf(2, samplesPerBlock);
    buf.clear();
    bufferSender.push(buf);
    bufferSender.push(buf);

#if DO_SINE_DEBUG
    auto func = [](float x) { return std::sin(x); };
    osc.initialise(func);
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;
    osc.prepare(spec);
    osc.setFrequency(440.f);
#endif
}

void Pfmcpp_project10AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Pfmcpp_project10AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Pfmcpp_project10AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
//    auto totalNumInputChannels  = getTotalNumInputChannels();

#if DO_SINE_DEBUG
    buffer.clear();
    auto numSamples = buffer.getNumSamples();
//    auto gain = Decibels::decibelsToGain(0.f, (float)NegativeInfinity);
    auto gain = Decibels::decibelsToGain(12.f, (float)NegativeInfinity);
//    auto gain = Decibels::decibelsToGain(3.f, (float)NegativeInfinity);
    for( int i = 0; i < numSamples; ++i )
    {
        auto sample = osc.processSample(0) * gain;
    #if DEBUG_IN_PHASE
        buffer.setSample(0, i, sample); //the left side.
        buffer.setSample(1, i, sample); //the right side.
    #else
        buffer.setSample(0, i, sample); //the left side.
        buffer.setSample(1, i, -sample); //the right side.
    #endif
    }
#endif

    if( auto* ph = getPlayHead() )
    {
        juce::AudioPlayHead::CurrentPositionInfo cpi;
        if( ph->getCurrentPosition(cpi) )
        {
            positionInfoSender.push(cpi);
        }
    }
//    buffer.applyGain(Decibels::decibelsToGain(6.f, (float)NegativeInfinity));
    bufferSender.push(buffer);
}

//==============================================================================
bool Pfmcpp_project10AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Pfmcpp_project10AudioProcessor::createEditor()
{
    return new Pfmcpp_project10AudioProcessorEditor (*this);
}

//==============================================================================
void Pfmcpp_project10AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    MemoryOutputStream mos(destData, false);
    DBG( settings.toXmlString() );
    settings.writeToStream(mos);
}

void Pfmcpp_project10AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = ValueTree::readFromData(data, sizeInBytes);
    DBG( tree.toXmlString() );
    if( tree.isValid() )
    {
        settings = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Pfmcpp_project10AudioProcessor();
}

