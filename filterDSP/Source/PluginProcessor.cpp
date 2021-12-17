/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterDSPAudioProcessor::FilterDSPAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), lowPassIIR(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.f, 0.1f))
#endif
{
}

FilterDSPAudioProcessor::~FilterDSPAudioProcessor()
{
}

//==============================================================================
const juce::String FilterDSPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterDSPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterDSPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterDSPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterDSPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterDSPAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterDSPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterDSPAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FilterDSPAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterDSPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FilterDSPAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();
    
    stateVariableFilter.reset();
    updateFilter();
    stateVariableFilter.prepare(spec);
    
    lowPassIIR.prepare(spec);
    lowPassIIR.reset();
    updateFilter();
    
    
}

void FilterDSPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void FilterDSPAudioProcessor::updateFilter()
{
    int choice = 0;  //da parametri
    int cutoff = 500; //da parametri
    int res = 2; //da parametri
    
    // 0 = lowpass
    if (choice == 0)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        stateVariableFilter.state->setCutOffFrequency(getSampleRate(), cutoff, res);
    }
    
    // 1 = bandpass
    if (choice == 1)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        stateVariableFilter.state->setCutOffFrequency(getSampleRate(), cutoff, res);
    }
    
    // 2 = highpass
    if (choice == 1)
    {
        stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        stateVariableFilter.state->setCutOffFrequency(getSampleRate(), cutoff, res);
    }
    
    // IIRfilter
    
    //Uno dei due:
   // lowPassIIR.state->makeLowPass(getSampleRate(), cutoff, res);
    *lowPassIIR.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), cutoff, res);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterDSPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FilterDSPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    updateFilter();
    
    stateVariableFilter.process(context);
    
    lowPassIIR.process(context);
    
    
}

//==============================================================================
bool FilterDSPAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FilterDSPAudioProcessor::createEditor()
{
    return new FilterDSPAudioProcessorEditor (*this);
}

//==============================================================================
void FilterDSPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FilterDSPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterDSPAudioProcessor();
}
