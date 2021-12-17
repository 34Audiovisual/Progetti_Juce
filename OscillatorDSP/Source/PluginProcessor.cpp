/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OscillatorDSPAudioProcessor::OscillatorDSPAudioProcessor()
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

OscillatorDSPAudioProcessor::~OscillatorDSPAudioProcessor()
{
}

//==============================================================================
const juce::String OscillatorDSPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OscillatorDSPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OscillatorDSPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OscillatorDSPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OscillatorDSPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OscillatorDSPAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OscillatorDSPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OscillatorDSPAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OscillatorDSPAudioProcessor::getProgramName (int index)
{
    return {};
}

void OscillatorDSPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OscillatorDSPAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec specs;
    specs.sampleRate = sampleRate;
    specs.maximumBlockSize = samplesPerBlock;
    
    osc.prepare(specs);
    osc.reset();
    osc.setFreq(440);
    
    rev.prepare(specs);
    rev.reset();
    
    
    delay.prepare(specs);
    delay.reset();
    delay.setDelay(44100);

    allpass.prepare(specs);
    allpass.reset();
    allpass.setType(juce::dsp::FirstOrderTPTFilterType::allpass);
    
    lowpass.prepare(specs);
    lowpass.reset();
    lowpass.setMode(juce::dsp::LadderFilterMode::LPF24);
    lowpass.setCutoffFrequencyHz(500);
    
    
    coeff.makeLowPass(sampleRate, 200);
    
    filter.prepare(specs);
    filter.reset();
    juce::dsp::FilterDesign<float> c;
    filter.coefficients =
    
    
}

void OscillatorDSPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OscillatorDSPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void OscillatorDSPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    //lowpass.process(context);
    //osc.process(context);
    //allpass.process(context);
    //filter.process(context);
    
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool OscillatorDSPAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OscillatorDSPAudioProcessor::createEditor()
{
    return new OscillatorDSPAudioProcessorEditor (*this);
}

//==============================================================================
void OscillatorDSPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OscillatorDSPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OscillatorDSPAudioProcessor();
}
