/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OscillatoreSinAudioProcessor::OscillatoreSinAudioProcessor()
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
}

OscillatoreSinAudioProcessor::~OscillatoreSinAudioProcessor()
{
}

//==============================================================================
const juce::String OscillatoreSinAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OscillatoreSinAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OscillatoreSinAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OscillatoreSinAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OscillatoreSinAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OscillatoreSinAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OscillatoreSinAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OscillatoreSinAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OscillatoreSinAudioProcessor::getProgramName (int index)
{
    return {};
}

void OscillatoreSinAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OscillatoreSinAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    updateAngleDelta();
}

void OscillatoreSinAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OscillatoreSinAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void OscillatoreSinAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto level = 0.125f;
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto* leftBuffer  = buffer.getWritePointer (0);
    auto* rightBuffer = buffer.getWritePointer (1);
    
           for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
           {
               auto currentSample = (float) std::sin (currentAngle);
               currentAngle += angleDelta;
               leftBuffer[sample]  = currentSample * level;
               rightBuffer[sample] = currentSample * level;
           }
    
/*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel, 0);

        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto currentSample = (float) std::sin (currentAngle);
            currentAngle += angleDelta;
            channelData[sample] = currentSample * level;
        }
    }
 */
}

//==============================================================================
bool OscillatoreSinAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OscillatoreSinAudioProcessor::createEditor()
{
    return new OscillatoreSinAudioProcessorEditor (*this);
}

//==============================================================================
void OscillatoreSinAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OscillatoreSinAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OscillatoreSinAudioProcessor();
}


juce::AudioProcessorValueTreeState::ParameterLayout OscillatoreSinAudioProcessor::createParam()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> param;
    param.push_back(std::make_unique<juce::AudioProcessorValueTreeState::Parameter::AudioParameterFloat>("FREQ", "Freq", 50.f, 10000.f, 440.f));
    
    return {param.begin(), param.end()};
}
