/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMoscAudioProcessor::FMoscAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),  apvts(*this, nullptr, "Parameters", createParam())
#endif
{
    apvts.addParameterListener("GAIN", this);
    apvts.addParameterListener("CARRIER", this);
    apvts.addParameterListener("MODULATOR", this);
    apvts.addParameterListener("INDEX", this);
}

FMoscAudioProcessor::~FMoscAudioProcessor()
{
}
void FMoscAudioProcessor::parameterChanged(const juce::String& paramID, float newValue)
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
const juce::String FMoscAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FMoscAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FMoscAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FMoscAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FMoscAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FMoscAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FMoscAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FMoscAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FMoscAudioProcessor::getProgramName (int index)
{
    return {};
}

void FMoscAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FMoscAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    freqCarr = *apvts.getRawParameterValue("MODULATOR");
    freqMod = *apvts.getRawParameterValue("CARRIER");
    idx = *apvts.getRawParameterValue("INDEX");
    gainV = *apvts.getRawParameterValue("GAIN");
    
    
    fm.prepare(sampleRate, freqCarr, freqMod, idx, SimpleFM::PM);
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();
    
    gain.reset();
    gain.prepare(spec);
    gain.setGainLinear(gainV);
}

void FMoscAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FMoscAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FMoscAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    fm.update(freqCarr, freqMod, idx);
    fm.renderBlock(buffer);
    
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    
    gain.setGainLinear(gainV);
    gain.process(context);
}

//==============================================================================
bool FMoscAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FMoscAudioProcessor::createEditor()
{
    return new FMoscAudioProcessorEditor (*this);
}

//==============================================================================
void FMoscAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FMoscAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FMoscAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout FMoscAudioProcessor::createParam ()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 1.f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CARRIER", "CarrierFreq", 0.0f, 5000.f, 111.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODULATOR", "ModulatorFreq", 0.0f, 5000.f, 110.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("INDEX", "Index", 0.0f, 10.f, 5.f));
    
    return { params.begin(), params.end() };
    
}
