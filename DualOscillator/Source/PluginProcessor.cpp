/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DualOscillatorAudioProcessor::DualOscillatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

DualOscillatorAudioProcessor::~DualOscillatorAudioProcessor()
{
}

//==============================================================================
const juce::String DualOscillatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DualOscillatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DualOscillatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DualOscillatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DualOscillatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DualOscillatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DualOscillatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DualOscillatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DualOscillatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void DualOscillatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DualOscillatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    synth.setCurrentPlaybackSampleRate (sampleRate);

     for (int i = 0; i < synth.getNumVoices(); i++)
     {
         if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
         {
             voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
         }
     }
}

void DualOscillatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DualOscillatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DualOscillatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateOsc();

    for (const juce::MidiMessageMetadata metadata : midiMessages)
        if (metadata.numBytes == 3)
            juce::Logger::writeToLog(metadata.getMessage().getDescription());

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool DualOscillatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DualOscillatorAudioProcessor::createEditor()
{
    return new DualOscillatorAudioProcessorEditor (*this);
}

//==============================================================================
void DualOscillatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DualOscillatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DualOscillatorAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout DualOscillatorAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //choice
    params.push_back(std::make_unique<juce::AudioParameterChoice>("ONOFF", "OnOff", juce::StringArray {"Off", "On"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("DIRTY", "Dirty", juce::StringArray {"0.01V", "0.1V", "1V", "10V"}, 2));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("WAVEFORM", "Waveform", juce::StringArray {"Sine", "Saw"}, 0));
    
    // KNOB
    params.push_back (std::make_unique<juce::AudioParameterFloat>("MODULATIONFREQ", "modulation", juce::NormalisableRange<float> { 0.0f, 25.0f, 0.1f }, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("FREQDEVIATION", "Deviation", juce::NormalisableRange<float> { 0.0f, 100.0f, 0.1f }, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("DUTYCYCLE", "DutyC", juce::NormalisableRange<float> { 0.0f, 100.0f, 0.1f }, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OUTPUT", "Output", juce::NormalisableRange<float> { 0.0f, 1.0f}, 0.0f));
    
    // FREQ
    params.push_back (std::make_unique<juce::AudioParameterFloat>("FREQ", "Freq", juce::NormalisableRange<float> { 0.0f, 7000.0f, 0.01f }, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("FREQFINE", "FreqFine", juce::NormalisableRange<float> { -100.0f, 100.f, 0.01f }, 0.0f));
    
    return { params.begin(), params.end() };
}
