/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AresAudioProcessor::AresAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),  apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    apvts.addParameterListener("ONOFF", this);
    apvts.addParameterListener("GAIN", this);
    apvts.addParameterListener("FREQ", this);
    apvts.addParameterListener("INDEX", this);
    apvts.addParameterListener("EVOLVE", this);
    apvts.addParameterListener("REVERB", this);
    apvts.addParameterListener("RANDOM", this);
    
    synth.addSound(new SynthSound());
    
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->setGain(*apvts.getRawParameterValue("GAIN"));
            voice->setIndex(*apvts.getRawParameterValue("INDEX"));
            voice->setCarrFreq(*apvts.getRawParameterValue("FREQ"));
        }
    }
    setEvTime(*apvts.getRawParameterValue("EVOLVE"));
    
    startTimer(30);
}

AresAudioProcessor::~AresAudioProcessor()
{
    stopTimer();
}

void AresAudioProcessor::parameterChanged(const juce::String& paramID, float newValue)
{
    if (paramID == "FREQ")
    {
        freqC = newValue;
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setCarrFreq(freqC);
            }
        }
    }
    
    if (paramID == "INDEX")
    {
        indexMod = newValue;
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setIndex(indexMod);
            }
        }

    }
    
    if (paramID == "ONOFF")
    {
        onOff = newValue;
        timeFromZero = 0;
        if (! onOff)
            midiBuf.addEvent(juce::MidiMessage::allNotesOff(1), 100);
    }
    
    if (paramID == "GAIN")
    {
        gain = newValue;
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setGain(newValue);
            }
        }
    }
    
    if (paramID == "REVERB")
    {
        for (int i = 0; i < synth.getNumVoices(); i++)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            {
                voice->setReverb(newValue);
            }
        }
    }
    
    if (paramID == "EVOLVE")
    {
        setEvTime(newValue);
    }
    
    if (paramID == "RANDOM")
    {
        random = newValue;
    }
    
}

//==============================================================================
const juce::String AresAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AresAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AresAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AresAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AresAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AresAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AresAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AresAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AresAudioProcessor::getProgramName (int index)
{
    return {};
}

void AresAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AresAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    synth.setCurrentPlaybackSampleRate (sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
    
    midiBuf.clear(0, samplesPerBlock);
}

void AresAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AresAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AresAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (! midiBuf.isEmpty())
    {
        midiMessages.swapWith(midiBuf);
        midiBuf.clear();
    }
    for (const juce::MidiMessageMetadata metadata : midiMessages)
        if (metadata.numBytes == 3)
            juce::Logger::writeToLog(metadata.getMessage().getDescription());
    
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool AresAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AresAudioProcessor::createEditor()
{
    return new AresAudioProcessorEditor (*this);
}

//==============================================================================
void AresAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, false);
    DBG( "WRITE : " << apvts.state.toXmlString() );
    apvts.state.writeToStream(mos);
}

void AresAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
        DBG( "READ : " << tree.toXmlString() );
        if( tree.isValid() )
        {
            apvts.state = tree;
        }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AresAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AresAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Bool

    params.push_back(std::make_unique<juce::AudioParameterBool>("ONOFF", "OnOff", false, "ON/OFF"));
    params.push_back(std::make_unique<juce::AudioParameterBool>("REVERB", "Reverb", true, "REV"));
    params.push_back(std::make_unique<juce::AudioParameterBool>("RANDOM", "RandomEv", false, "RND"));
    
    // Float
    params.push_back (std::make_unique<juce::AudioParameterFloat>("INDEX", "Index", juce::NormalisableRange<float> { 0.0f, 2.0f, 0.01f }, 1.f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("FREQ", "Frequency", juce::NormalisableRange<float> { 10.0f, 2000.0f, 0.1f, 0.3, false }, 20.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.2f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("EVOLVE", "Evolve", juce::NormalisableRange<float> { 1.0f, 10.0f, 0.1f }, 5.f));


    
    return { params.begin(), params.end() };
}
