/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GranulatorAudioProcessor::GranulatorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, juce::Identifier("GranulatorParameters"), createParameterLayout())
#endif
{
    parameters.addParameterListener(NAME_POSITIONL, this);
    parameters.addParameterListener(NAME_POSITIONR, this);
    parameters.addParameterListener(NAME_SPREAD, this);
    parameters.addParameterListener(NAME_GRAIN_LENGHT, this);
    parameters.addParameterListener(NAME_GRAIN_PITCH, this);
    parameters.addParameterListener(NAME_ONOFF, this);
    parameters.addParameterListener(NAME_RANDOM, this);
    parameters.addParameterListener(NAME_LINK, this);
    parameters.addParameterListener(NAME_MONO, this);
    parameters.addParameterListener(NAME_NUM_VOICES, this);
    parameters.addParameterListener(NAME_DELTAPOS, this);
    parameters.addParameterListener(NAME_DELTAPITCH, this);
    parameters.addParameterListener(NAME_DELTALEN, this);
    
    setting.setProperty("path", "path", nullptr);
    
    loadedFile.clear();
   // prepareGranulator();
}

GranulatorAudioProcessor::~GranulatorAudioProcessor()
{
}

void GranulatorAudioProcessor::parameterChanged(const juce::String& paramID, float newValue)
{
    juce::Random r;
    
    if (paramID == NAME_POSITIONL)
    {
        if (mono)
        {
            granulatorsL.setPosition(newValue/100.f);
            parameters.getParameterAsValue(NAME_POSITIONR).setValue(newValue);
        }
        else if (link)
        {
            granulatorsL.setPosition(newValue/100.f);
            parameters.getParameterAsValue(NAME_POSITIONR).setValue(newValue - linkedDiff);
        }
        else
        {
            granulatorsL.setPosition(newValue/100.f);
        }
    }
    
    if (paramID == NAME_POSITIONR)
    {
        if (mono)
        {
            granulatorsR.setPosition(newValue/100.f);
            parameters.getParameterAsValue(NAME_POSITIONL).setValue(newValue);
        }
        else if (link)
        {
            granulatorsR.setPosition(newValue/100.f);
            parameters.getParameterAsValue(NAME_POSITIONL).setValue(newValue + linkedDiff);
        }
        else
        {
            granulatorsR.setPosition(newValue/100.f);
        }
    }
    
    if (paramID == NAME_SPREAD)
    {
        spread = newValue / 100.f;
    }
    
    if (paramID == NAME_GRAIN_LENGHT)
    {
        granulatorsL.setGrainLenght(newValue);
        granulatorsR.setGrainLenght(newValue);
    }
    
    if (paramID == NAME_GRAIN_PITCH)
    {
        granulatorsL.setPitch(newValue/100.f);
        granulatorsR.setPitch(newValue/100.f);
    }
    
    if (paramID == NAME_ONOFF)
    {
        granulatorsL.setOn(newValue);
        granulatorsR.setOn(newValue);
    }
    
    if (paramID == NAME_RANDOM)
    {
        granulatorsL.setRandom(newValue);
        granulatorsR.setRandom(newValue);
    }
    
    if (paramID == NAME_MONO)
    {
        linkedDiff = 0;
        auto posN = parameters.getParameterAsValue(NAME_POSITIONL);
        parameters.getParameterAsValue(NAME_POSITIONR).setValue(posN);
        mono = newValue;
    }
    
    if (paramID == NAME_LINK)
    {
        link = newValue;
        auto L = (float)parameters.getParameterAsValue(NAME_POSITIONL).getValue();
        auto R = (float)parameters.getParameterAsValue(NAME_POSITIONR).getValue();
        linkedDiff = L - R;
    }
    
    if (paramID == NAME_NUM_VOICES && !loadedFile.hasBeenCleared())
    {

        if (!isChangingVoices)
        {
            nVoices = newValue;
            granulatorsL.setOn(false);
            granulatorsL.setFileLoaded(false);
            granulatorsR.setOn(false);
            granulatorsR.setFileLoaded(false);
            prepareGranulator ();
            isChangingVoices = false;
        }

    }
    
    if (paramID == NAME_DELTAPOS)
    {
        granulatorsL.setDeltaPos(newValue/100.f);
        granulatorsR.setDeltaPos(newValue/100.f);
    }
    
    if (paramID == NAME_DELTAPITCH)
    {
        granulatorsL.setDeltaPitch(newValue/100.f);
        granulatorsR.setDeltaPitch(newValue/100.f);
    }
    
    if (paramID == NAME_DELTALEN)
    {
        granulatorsL.setDeltaLen(newValue/100.f);
        granulatorsR.setDeltaLen(newValue/100.f);
    }
}
//==============================================================================
const juce::String GranulatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GranulatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GranulatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GranulatorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GranulatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GranulatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GranulatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GranulatorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GranulatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void GranulatorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GranulatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}

void GranulatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GranulatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GranulatorAudioProcessor::makePluginMono ()
{
    
}

void GranulatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (granulatorsL.gran0().isFileLoaded())
    {
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float levelSampleL = 0;
            float levelSampleR = 0;
            if (isPlaying)
            {
                levelSampleL = loadedFile.getSample(0, readIdx);
                levelSampleR = levelSampleL;
                readIdx += 1;
                readIdx %= loadedFile.getNumSamples();
            }
            else
            {
                if (granulatorsL.gran0().isActive())
                {
                    levelSampleL = granulatorsL.nextFloat();
                    levelSampleR = mono ? levelSampleL : granulatorsR.nextFloat();
                }
                
                if (readIdx != 0)
                    readIdx = 0;
            }
            
            if (!isPluginMono)
            {
                auto* leftBuffer  = buffer.getWritePointer (0);
                auto* rightBuffer = buffer.getWritePointer (1);
                leftBuffer[sample]  = 0.5f * (1.f + spread) *levelSampleL + 0.5f * (1.f - spread) *levelSampleR;
                rightBuffer[sample] = 0.5f * (1.f + spread) *levelSampleR + 0.5f * (1.f - spread) *levelSampleL;
            }
            else
            {
                auto* leftBuffer  = buffer.getWritePointer (0);
                leftBuffer[sample]  = levelSampleL;
            }
        }
    };
}

//==============================================================================
bool GranulatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GranulatorAudioProcessor::createEditor()
{
    return new GranulatorAudioProcessorEditor (*this);
}

//==============================================================================
void GranulatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

    
    setting.appendChild(parameters.state, nullptr);
    
    juce::MemoryOutputStream mos(destData, false);
    DBG( setting.toXmlString() );
    setting.writeToStream(mos);
}

void GranulatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    DBG( tree.toXmlString() );
    if( tree.isValid() )
    {
        setting = tree;
    }
    parameters.replaceState(setting.getChildWithName("GranulatorParameters"));
    
    path = setting.getProperty("path");
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GranulatorAudioProcessor();
}
//==============================================================================
void GranulatorAudioProcessor::prepareGranulator ()
{
    nVoices = *parameters.getRawParameterValue(NAME_NUM_VOICES);
    mono = *parameters.getRawParameterValue(NAME_MONO);
    link = *parameters.getRawParameterValue(NAME_LINK);
    spread = *parameters.getRawParameterValue(NAME_SPREAD)/100.f;
    
    float positionL = *parameters.getRawParameterValue(NAME_POSITIONL)/100.f;
    float positionR = *parameters.getRawParameterValue(NAME_POSITIONR)/100.f;
    float grainLenght = *parameters.getRawParameterValue(NAME_GRAIN_LENGHT);
    float gPitch = *parameters.getRawParameterValue(NAME_GRAIN_PITCH)/100.f;
    bool on = *parameters.getRawParameterValue(NAME_ONOFF);
    bool ran = *parameters.getRawParameterValue(NAME_RANDOM);
    float deltaPos = *parameters.getRawParameterValue(NAME_DELTAPOS)/100.f;
    float deltaLen = *parameters.getRawParameterValue(NAME_DELTALEN)/100.f;
    float deltaPitch = *parameters.getRawParameterValue(NAME_DELTAPITCH)/100.f;
    
    auto sampleRate = getSampleRate();
    
    granulatorsL.setParameters(sampleRate, grainLenght, positionL, gPitch, ran, on, deltaPos, deltaLen, deltaPitch);
    granulatorsR.setParameters(sampleRate, grainLenght, positionR, gPitch, ran, on, deltaPos, deltaLen, deltaPitch);
    
    //granulators.prepareGranulatorsVoices();
    
    //DBG("PREPARE!");
    
}
