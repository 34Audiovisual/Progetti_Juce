/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessor::DelayAudioProcessor()
     : parameters(*this, nullptr, juce::Identifier("DelayParameters"), createParameterLayout())

{
    parameters.addParameterListener(NAME_DW, this);
    parameters.addParameterListener(NAME_DELAY_TIME, this);
    parameters.addParameterListener(NAME_DELAY_TIME_L, this);
    parameters.addParameterListener(NAME_DELAY_TIME_R, this);
    parameters.addParameterListener(NAME_FB, this);
    parameters.addParameterListener(NAME_CC, this);
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

void DelayAudioProcessor::parameterChanged(const juce::String& paramID, float newValue)
{
    if (paramID == NAME_DW)
        dryWet = newValue;

    if (paramID == NAME_DELAY_TIME)
        delayT = newValue;

    if (paramID == NAME_DELAY_TIME_L)
        delayTL = newValue;

    if (paramID == NAME_DELAY_TIME_R)
        delayTR = newValue;

    if (paramID == NAME_FB)
        feedb = newValue;

    if (paramID == NAME_CC)
        CC = newValue > 0.5f;
}
//==============================================================================
const juce::String DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto biggerBufferSize = MAX_DELAY_TIME * sampleRate;
    circularBuffer.prepare(getTotalNumOutputChannels(), (int)biggerBufferSize);

    delay.prepare(sampleRate);
    delayL.prepare(sampleRate);
    delayR.prepare(sampleRate);
}

void DelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void DelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto bufferSize = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    juce::AudioBuffer<float> bufferElab;
    bufferElab.makeCopyOf(buffer);

    if (CC == true)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = bufferElab.getWritePointer(channel);

            delay.updateDelay(delayT, feedb);
            delay.getFromBiggerBuffer(bufferElab, circularBuffer.getBiggerBuffer(), circularBuffer.getWritePosition(), channel);
            circularBuffer.fillbuffer(channel, bufferSize, channelData, bufferElab);

        }

        delay.getFromBiggerBuffer(bufferElab, circularBuffer.getBiggerBuffer(), circularBuffer.getWritePosition(), 0);

        circularBuffer.nextCycle(bufferElab.getNumSamples());
    }

    else
    {
        auto* channelDataL = bufferElab.getWritePointer(0);
        auto* channelDataR = bufferElab.getWritePointer(1);

        delayL.updateDelay(delayTL, feedb);
        delayL.getFromBiggerBuffer(bufferElab, circularBuffer.getBiggerBuffer(), circularBuffer.getWritePosition(), 0);


        delayR.updateDelay(delayTR, feedb);
        delayR.getFromBiggerBuffer(bufferElab, circularBuffer.getBiggerBuffer(), circularBuffer.getWritePosition(), 1);

        circularBuffer.fillbuffer(0, bufferSize, channelDataL, bufferElab);
        circularBuffer.fillbuffer(1, bufferSize, channelDataR, bufferElab);

        circularBuffer.nextCycle(bufferElab.getNumSamples());
    }

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto dry = buffer.getSample(channel, sample);
            auto wet = bufferElab.getSample(channel, sample);

            buffer.setSample(channel, sample, (dry * (1 - dryWet) + wet * dryWet));
        }

    }

}

//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return new DelayAudioProcessorEditor (*this);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}
