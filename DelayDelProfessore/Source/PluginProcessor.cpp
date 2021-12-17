#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayEffectAudioProcessor::DelayEffectAudioProcessor() :
    parameters(*this, nullptr, Identifier("DelayParameters"), createParameterLayout() )
{
    parameters.addParameterListener(NAME_DW, &dryWet);
    parameters.addParameterListener(NAME_DELAY_TIME, &delay);
    parameters.addParameterListener(NAME_FB, &delay);
    parameters.addParameterListener(NAME_RATE, &osc);
    parameters.addParameterListener(NAME_AMT, this);
    parameters.addParameterListener(NAME_WF, &osc);

    modulationAmount.setCurrentAndTargetValue(DEFAULT_AMT);
}

DelayEffectAudioProcessor::~DelayEffectAudioProcessor()
{
}

void DelayEffectAudioProcessor::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == NAME_AMT)
        modulationAmount.setTargetValue(newValue);
}

//==============================================================================
void DelayEffectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dryWet.prepareToPlay(samplesPerBlock);
    delay.prepareToPlay(samplesPerBlock, sampleRate);
    osc.pepareToPlay(sampleRate);

    modulator.setSize(1, samplesPerBlock);

    modulationAmount.reset(sampleRate, RATE_SMTH);
}

void DelayEffectAudioProcessor::releaseResources()
{
    dryWet.releaseResources();
    delay.releaseResources();
    modulator.setSize(0, 0);
}

void DelayEffectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    const auto numSamples = buffer.getNumSamples();

    osc.getNextAudioBlock(modulator, numSamples);

    scaleModulation(numSamples);

    dryWet.storeFrom(buffer);

    delay.processBlock(buffer, modulator);
    
    dryWet.mergeTo(buffer);

    // ~~~ Listen to the wavez! ~~~
    //osc.getNextAudioBlock(buffer, buffer.getNumSamples());

}

void DelayEffectAudioProcessor::scaleModulation(int numSamples)
{
    auto* modData = modulator.getWritePointer(0);

    FloatVectorOperations::add(modData, 1.0f, numSamples);
    FloatVectorOperations::multiply(modData, 0.5f, numSamples);
    modulationAmount.applyGain(modData, numSamples);

}

//==============================================================================
bool DelayEffectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayEffectAudioProcessor::createEditor()
{
    return new PluginEditor(*this, parameters);
}

//==============================================================================
void DelayEffectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DelayEffectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new DelayEffectAudioProcessor();
}
