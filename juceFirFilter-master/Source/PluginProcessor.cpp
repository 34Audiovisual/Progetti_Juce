/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
JuceFirfilterExampleAudioProcessor::JuceFirfilterExampleAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), tree(*this, nullptr), lowPassFilter(dsp::FilterDesign<float>::designFIRLowpassWindowMethod(20000.0f, 44100, 21, dsp::WindowingFunction <float>::hamming))
#endif
{
    NormalisableRange<float> cutoffRange (20.0f, 20000.0f);
    tree.createAndAddParameter("cutoff", "Cutoff", "cutoff", cutoffRange, 600.0f, nullptr, nullptr);
}

JuceFirfilterExampleAudioProcessor::~JuceFirfilterExampleAudioProcessor()
{
}

//==============================================================================
const String JuceFirfilterExampleAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuceFirfilterExampleAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JuceFirfilterExampleAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JuceFirfilterExampleAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JuceFirfilterExampleAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuceFirfilterExampleAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JuceFirfilterExampleAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuceFirfilterExampleAudioProcessor::setCurrentProgram (int index)
{
}

const String JuceFirfilterExampleAudioProcessor::getProgramName (int index)
{
    return {};
}

void JuceFirfilterExampleAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void JuceFirfilterExampleAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;
    
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    lowPassFilter.reset();
    updateFilter();
    lowPassFilter.prepare(spec);
}

void JuceFirfilterExampleAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JuceFirfilterExampleAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void JuceFirfilterExampleAudioProcessor::updateFilter()
{
    float freq = *tree.getRawParameterValue("cutoff");
    
    *lowPassFilter.state = *dsp::FilterDesign<float>::designFIRLowpassWindowMethod(freq, 44100, 21, dsp::WindowingFunction <float>::hamming);
}

void JuceFirfilterExampleAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    dsp::AudioBlock<float> block (buffer);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    updateFilter();
    lowPassFilter.process(dsp::ProcessContextReplacing<float> (block));
    
}

//==============================================================================
bool JuceFirfilterExampleAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* JuceFirfilterExampleAudioProcessor::createEditor()
{
    return new JuceFirfilterExampleAudioProcessorEditor (*this);
}

//==============================================================================
void JuceFirfilterExampleAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JuceFirfilterExampleAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceFirfilterExampleAudioProcessor();
}

