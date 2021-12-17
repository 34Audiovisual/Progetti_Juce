#include "PluginProcessor.h"

//==============================================================================
DelayAudioProcessor::DelayAudioProcessor() :
    parameters(*this, nullptr, Identifier("DelayParameters"), createParameterLayout())
{
    parameters.addParameterListener(NAME_DW, this);
    parameters.addParameterListener(NAME_CC, this);
    parameters.addParameterListener(NAME_DELAY_TIME, this);
    parameters.addParameterListener(NAME_DELAY_TIME_L, this);
    parameters.addParameterListener(NAME_DELAY_TIME_R, this);
    parameters.addParameterListener(NAME_FB, this);
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

void DelayAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == NAME_CC)
        CC = newValue > 0.5f;
    
    if (paramID == NAME_DELAY_TIME)
        delayT = newValue;
    
    if (paramID == NAME_DELAY_TIME_L)
        delayTL = newValue;
    
    if (paramID == NAME_DELAY_TIME_R)
        delayTR = newValue;
    
    if (paramID == NAME_FB)
        feedb = newValue;
    
    if (paramID == NAME_DW)
        dryWet = newValue;

}

//==============================================================================
void DelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    //dryWet.prepareToPlay(samplesPerBlock);
    auto biggerBufferSize = MAX_DELAY_TIME * sampleRate;
    circularBuffer.prepare(getTotalNumOutputChannels(), (int)biggerBufferSize);
    
    delay.prepare(sampleRate);
    delayL.prepare(sampleRate);
    delayR.prepare(sampleRate);
    //delay.setDelayGain(0.99f);
}

void DelayAudioProcessor::releaseResources()
{
    //dryWet.releaseResources();
}

void DelayAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
            auto* channelData = bufferElab.getWritePointer (channel);


            delay.updateDelay(delayT, feedb);
            delay.getFromBiggerBuffer(bufferElab, circularBuffer.getBiggerBuffer(), circularBuffer.getWritePosition(), channel);
            circularBuffer.fillbuffer(channel, bufferSize, channelData, bufferElab);

        }
      
      delay.getFromBiggerBuffer(bufferElab, circularBuffer.getBiggerBuffer(), circularBuffer.getWritePosition(), 0);
      
      circularBuffer.nextCycle(bufferElab.getNumSamples());
    }
    
    else
    {
        auto* channelDataL = bufferElab.getWritePointer (0);
        auto* channelDataR = bufferElab.getWritePointer (1);
        
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
            
            buffer.setSample(channel, sample, (dry * ( 1 - dryWet) + wet * dryWet));
        }
        
    }

}

//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return nullptr;
}

//==============================================================================
void DelayAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DelayAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}
