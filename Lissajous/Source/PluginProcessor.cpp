/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LissajousAudioProcessor::LissajousAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, juce::Identifier("OscParameters"), createParameterLayout() )
#endif
{
    parameters.addParameterListener(NAME_AMP_OSCX, this);
    parameters.addParameterListener(NAME_PHASE_OSCX, this);
    parameters.addParameterListener(NAME_FREQ_OSCX, this);
    parameters.addParameterListener(NAME_MULT_OSCX, this);
    parameters.addParameterListener(NAME_AMP_OSCY, this);
    parameters.addParameterListener(NAME_PHASE_OSCY, this);
    parameters.addParameterListener(NAME_FREQ_OSCY, this);
    parameters.addParameterListener(NAME_MULT_OSCY, this);
    parameters.addParameterListener(NAME_DIM, this);
    parameters.addParameterListener(NAME_OFFSETX, this);
    parameters.addParameterListener(NAME_OFFSETY, this);

}

LissajousAudioProcessor::~LissajousAudioProcessor()
{
}

void LissajousAudioProcessor::parameterChanged(const juce::String& paramID, float newValue)
{
    if (paramID == NAME_AMP_OSCX)
        oscX.changeLevel(newValue);
    
    if (paramID == NAME_PHASE_OSCX)
    {
        auto newPhase = juce::MathConstants<float>::pi * newValue;
        oscX.phaseMod(newPhase);
    }
    
    if (paramID == NAME_FREQ_OSCX)
        oscX.changeFreq(newValue);
    
    if (paramID == NAME_MULT_OSCX)
        oscX.changeMult(newValue);
    
    if (paramID == NAME_AMP_OSCY)
        oscY.changeLevel(newValue);
    
    if (paramID == NAME_PHASE_OSCY)
    {
        auto newPhase = juce::MathConstants<float>::pi * newValue;
        oscY.phaseMod(newPhase);
    }
    
    if (paramID == NAME_FREQ_OSCY)
        oscY.changeFreq(newValue);
    
    if (paramID == NAME_MULT_OSCY)
        oscY.changeMult(newValue);
    
    if (paramID == NAME_DIM)
    {
        oscX.changeLevelGen(newValue);
        oscY.changeLevelGen(newValue);
    }
    
    if (paramID == NAME_OFFSETX)
        oscX.setOffset(newValue);
    
    if (paramID == NAME_OFFSETY)
        oscY.setOffset(newValue);

}

//==============================================================================
const juce::String LissajousAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LissajousAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LissajousAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LissajousAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LissajousAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LissajousAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LissajousAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LissajousAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LissajousAudioProcessor::getProgramName (int index)
{
    return {};
}

void LissajousAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LissajousAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oscX.prepare(sampleRate, *parameters.getRawParameterValue(NAME_FREQ_OSCX) );
    oscY.prepare(sampleRate, *parameters.getRawParameterValue(NAME_FREQ_OSCY));
    
    oscX.phaseMod(*parameters.getRawParameterValue(NAME_PHASE_OSCX));
    oscY.phaseMod(*parameters.getRawParameterValue(NAME_PHASE_OSCY));
    
    oscX.changeLevel(*parameters.getRawParameterValue(NAME_AMP_OSCX));
    oscY.changeLevel(*parameters.getRawParameterValue(NAME_AMP_OSCY));
    
    oscX.changeLevelGen(*parameters.getRawParameterValue(NAME_DIM));
    oscY.changeLevelGen(*parameters.getRawParameterValue(NAME_DIM));
    
    oscX.changeMult(*parameters.getRawParameterValue(NAME_MULT_OSCX));
    oscY.changeMult(*parameters.getRawParameterValue(NAME_MULT_OSCY));
    
}

void LissajousAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LissajousAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
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

void LissajousAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        buffer.setSample(0, sample, oscX.nextFloat());
        buffer.setSample(1, sample, oscY.nextFloat());
    }

    bufferExt.makeCopyOf(buffer);
}

//==============================================================================
bool LissajousAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LissajousAudioProcessor::createEditor()
{
    return new LissajousAudioProcessorEditor (*this);
}

//==============================================================================
void LissajousAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void LissajousAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new LissajousAudioProcessor();
}
