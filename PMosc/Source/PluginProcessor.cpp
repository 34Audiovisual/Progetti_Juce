/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PMoscAudioProcessor::PMoscAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParam())
#endif
{
    apvts.addParameterListener("GAIN", this);
    apvts.addParameterListener("CARRIER", this);
    apvts.addParameterListener("MODULATOR", this);
    apvts.addParameterListener("INDEX", this);
}

PMoscAudioProcessor::~PMoscAudioProcessor()
{
}

void PMoscAudioProcessor::parameterChanged(const juce::String& paramID, float newValue)
{
    if (paramID == "GAIN")
        gainV = newValue;
    
    if (paramID == "CARRIER")
        freqCarr = newValue;
    
    if (paramID == "MODULATOR")
        freqMod = newValue;
    
    if (paramID == "INDEX")
        idx = newValue;

}
//==============================================================================
const juce::String PMoscAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PMoscAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PMoscAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PMoscAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PMoscAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PMoscAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PMoscAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PMoscAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PMoscAudioProcessor::getProgramName (int index)
{
    return {};
}

void PMoscAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PMoscAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    float carrFreq = 839.f;
    float modFreq = 739.f;
    float index = 2.f;
    
    pm.prepare(sampleRate, carrFreq, modFreq, index);
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();
    
    gain.reset();
    gain.prepare(spec);
    gain.setGainLinear(gainV);
}

void PMoscAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PMoscAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PMoscAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    pm.update(freqCarr, freqMod, idx);
    pm.renderBlock(buffer);
    
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    
    gain.setGainLinear(gainV);
    gain.process(context);
}

//==============================================================================
bool PMoscAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PMoscAudioProcessor::createEditor()
{
    return new PMoscAudioProcessorEditor (*this);
}

//==============================================================================
void PMoscAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PMoscAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PMoscAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout PMoscAudioProcessor::createParam ()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 1.f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CARRIER", "CarrierFreq", 0.0f, 5000.f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODULATOR", "ModulatorFreq", 0.0f, 5000.f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("INDEX", "Index", 0.0f, 10.f, 2.f));
    
    return { params.begin(), params.end() };
    
}
