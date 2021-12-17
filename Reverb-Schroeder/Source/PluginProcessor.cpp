/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverbSchroederAudioProcessor::ReverbSchroederAudioProcessor()
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
}

ReverbSchroederAudioProcessor::~ReverbSchroederAudioProcessor()
{

}

//==============================================================================
const juce::String ReverbSchroederAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbSchroederAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbSchroederAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbSchroederAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbSchroederAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbSchroederAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverbSchroederAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbSchroederAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReverbSchroederAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverbSchroederAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReverbSchroederAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    smRate = sampleRate;
    
    juce::dsp::ProcessSpec specs;
    specs.sampleRate = sampleRate;
    specs.maximumBlockSize = samplesPerBlock;
    
    lowpass.prepare(specs);
    lowpass.reset();
    lowpass.setMode(juce::dsp::LadderFilterMode::LPF24);
    lowpass.setCutoffFrequencyHz(7000);
    
    highpass.prepare(specs);
    highpass.reset();
    highpass.setMode(juce::dsp::LadderFilterMode::HPF24);
    highpass.setCutoffFrequencyHz(30);
    
    setDecay(100.f);
    
    allpass1L.prepare(5, 96.83, smRate);
    allpass2L.prepare(1.7, 32.92, smRate);
    allpass1R.prepare(6.1, 90.83, smRate);
    allpass2R.prepare(1.6, 38.92, smRate);
    
    early.prepare(smRate);
    
}

void ReverbSchroederAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbSchroederAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ReverbSchroederAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    //Clear other channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
    
    //comb1.process(buffer);
    
    juce::AudioBuffer<float> buffToProcess;
    buffToProcess.makeCopyOf(buffer);
    
    //Eq
    juce::dsp::AudioBlock<float> block (buffToProcess);
    juce::dsp::ProcessContextReplacing<float> context (block);
    lowpass.process(context);
    highpass.process(context);

    juce::AudioBuffer<float> earlyRef = early.process(buffer);
    
    buffToProcess.addFrom(0, 0, earlyRef, 0, 0, buffer.getNumSamples());
    buffToProcess.addFrom(1, 0, earlyRef, 1, 0, buffer.getNumSamples());
    
    
    // Comb Filter process
    
    juce::AudioBuffer<float> tempBuff1;
    tempBuff1.makeCopyOf(buffToProcess);
    comb1.process(tempBuff1);
    
    juce::AudioBuffer<float> tempBuff2;
    tempBuff2.makeCopyOf(buffToProcess);
    comb2.process(tempBuff2);
    
    juce::AudioBuffer<float> tempBuff3;
    tempBuff3.makeCopyOf(buffToProcess);
    comb3.process(tempBuff3);
    
    juce::AudioBuffer<float> tempBuff4;
    tempBuff4.makeCopyOf(buffToProcess);
    comb4.process(tempBuff4);
    
    for (int channel = 0 ; channel < totalNumInputChannels; ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples() ; ++sample)
        {
            auto tSample1 =  (tempBuff1.getSample(channel, sample) + tempBuff2.getSample(channel, sample) + tempBuff3.getSample(channel, sample) + tempBuff4.getSample(channel, sample)) * 0.25;
            
            buffToProcess.setSample(channel, sample, tSample1);
        }
        
        
    }
    
    // allpass filter process
    
    juce::AudioBuffer<float> tempBuff5;
    tempBuff5.makeCopyOf(buffToProcess);
    allpass1L.process(tempBuff5);
    allpass2L.process(tempBuff5);
    
    juce::AudioBuffer<float> tempBuff6;
    tempBuff6.makeCopyOf(buffToProcess);
    allpass1R.process(tempBuff6);
    allpass2R.process(tempBuff6);

    for (int channel = 0 ; channel < totalNumInputChannels; ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples() ; ++sample)
        {
            auto sampleTowrite = buffer.getSample(channel, sample) * (1 - dryWet) + (earlyRef.getSample(channel, sample) + buffToProcess.getSample(channel, sample)) * dryWet;
            
            buffer.setSample(channel, sample, sampleTowrite);
        }
        
        
    }
    

}

//==============================================================================
bool ReverbSchroederAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbSchroederAudioProcessor::createEditor()
{
    return new ReverbSchroederAudioProcessorEditor (*this);
}

//==============================================================================
void ReverbSchroederAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ReverbSchroederAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbSchroederAudioProcessor();
}


juce::AudioProcessorValueTreeState::ParameterLayout ReverbSchroederAudioProcessor::createParam ()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 5000.f, 100.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRYWET", "DryWet", 0.0f, 1.f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("EARLY", "Early", 0.0f, 1.f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOWPASS", "LowPass", 200.0f, 15000.f, 7000.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIGHPASS", "HighPass", 20.0f, 500.f, 30.f));
        
    return { params.begin(), params.end() };
        
}
    

