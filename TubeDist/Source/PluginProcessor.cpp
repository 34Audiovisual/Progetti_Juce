/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TubeDistAudioProcessor::TubeDistAudioProcessor()
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

TubeDistAudioProcessor::~TubeDistAudioProcessor()
{
}

//==============================================================================
const juce::String TubeDistAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TubeDistAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TubeDistAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TubeDistAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TubeDistAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TubeDistAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TubeDistAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TubeDistAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TubeDistAudioProcessor::getProgramName (int index)
{
    return {};
}

void TubeDistAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TubeDistAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    float inGain = -2.f;
    float outGain = 0.1f;
    float drive = 0.1f; //tra -1.0f e 10.f
    float blend = -10.f; //blend (-10.f == 'tape' -> +10.f == 'tube')
    tube.setSampleRate(sampleRate);
    tube.feedCoontrols(inGain, outGain, drive, blend);
    
}

void TubeDistAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TubeDistAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TubeDistAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());



        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto currentSampleL = buffer.getSample(0, sample);
            auto currentSampleR = buffer.getSample(1, sample);
            buffer.setSample(0, sample, tube.perform(currentSampleL));
            buffer.setSample(1, sample, tube.perform(currentSampleR));
        }
}

//==============================================================================
bool TubeDistAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TubeDistAudioProcessor::createEditor()
{
    return new TubeDistAudioProcessorEditor (*this);
}

//==============================================================================
void TubeDistAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TubeDistAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TubeDistAudioProcessor();
}
