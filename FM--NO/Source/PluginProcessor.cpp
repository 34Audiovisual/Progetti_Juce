/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMAudioProcessor::FMAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

FMAudioProcessor::~FMAudioProcessor()
{
}

//==============================================================================
const juce::String FMAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FMAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FMAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FMAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FMAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FMAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FMAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FMAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FMAudioProcessor::getProgramName (int index)
{
    return {};
}

void FMAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FMAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.numChannels = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    carrier.reset();
    carrier.prepare(spec);
    modulator.reset();
    modulator.prepare(spec);
    gain.reset();
    gain.prepare(spec);
    
    carrier.initialise([] (float x) { return std::sin (x); }, 512);
    carrier.setFrequency(carrFreq);
    modulator.initialise([] (float x) { return std::sin (x); }, 512);
    modulator.setFrequency(modFreq);
    gain.setGainLinear(0.1);
}

void FMAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FMAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void FMAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block (buffer);
    
    juce::dsp::ProcessContextReplacing<float> context (block);
   
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        auto dev = index * modulator.getFrequency();
        DBG(dev);
        auto modSample = modulator.processSample(0);
        //auto currDisc = juce::jmap (modSample, -1.0f, 1.0f, 0.f, dev);
        auto currDisc = modSample * dev;
        auto currCarrFreq = carrFreq + currDisc;
        
        if (currCarrFreq < 0)
        {
            currCarrFreq = - currCarrFreq;
            ph = - 1.f;
        }
        
        else
        {
            ph =1.f;
        }

        //if (currCarrFreq > 0)
        carrier.setFrequency(0, true);
        auto s = carrier.processSample(0) * 0.5f;
        buffer.setSample(0, sample, s);
        buffer.setSample(1, sample, s);
    }
    //carrier.setFrequency(440);
    //carrier.process(context);
    //gain.process(context);
}

//==============================================================================
bool FMAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FMAudioProcessor::createEditor()
{
    return new FMAudioProcessorEditor (*this);
}

//==============================================================================
void FMAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FMAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FMAudioProcessor();
}


