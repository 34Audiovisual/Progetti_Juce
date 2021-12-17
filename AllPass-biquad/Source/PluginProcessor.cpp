/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AllPassbiquadAudioProcessor::AllPassbiquadAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

AllPassbiquadAudioProcessor::~AllPassbiquadAudioProcessor()
{

}

//==============================================================================
const juce::String AllPassbiquadAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AllPassbiquadAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AllPassbiquadAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AllPassbiquadAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AllPassbiquadAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AllPassbiquadAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AllPassbiquadAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AllPassbiquadAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AllPassbiquadAudioProcessor::getProgramName (int index)
{
    return {};
}

void AllPassbiquadAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AllPassbiquadAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    sin.reset();
    cos.reset();
    sin.prepare(spec);
    cos.prepare(spec);
    
    sin.initialise([] (float x) { return std::sin (x); }, 128);
    cos.initialise([] (float x) { return std::cos (x); }, 128);
    sin.setFrequency(1000);
    cos.setFrequency(1000);
    
    allpassL1.reset();
    allpassL1.prepare(spec);
    
    allpassL2.reset();
    allpassL2.prepare(spec);
    
    allpassR1.reset();
    allpassR1.prepare(spec);
    
    allpassR2.reset();
    allpassR2.prepare(spec);
    
    
    updateFilter();
}

void AllPassbiquadAudioProcessor::updateFilter()
{
    
    //Hilbet Trensform
    juce::dsp::IIR::Coefficients<float>::Ptr coeffsL1 (new juce::dsp::IIR::Coefficients<float>( -0.260502f, 0.02569f, 1.f, 1.f, 0.02569f, -0.260502f));
    
    juce::dsp::IIR::Coefficients<float>::Ptr coeffsL2 (new juce::dsp::IIR::Coefficients<float>( 0.870686f, -1.8685f, 1.f, 1.f, -1.8685f, 0.870686f));
    
    juce::dsp::IIR::Coefficients<float>::Ptr coeffsR1 (new juce::dsp::IIR::Coefficients<float>( 0.94657f, -1.94632f, 1.f, 1.f, -1.94632f, 0.94657f));
    
    juce::dsp::IIR::Coefficients<float>::Ptr coeffsR2 (new juce::dsp::IIR::Coefficients<float>( 0.06338f, -0.83774f, 1.f, 1.f, -0.83774f, 0.06338f));
    
    *allpassL1.state = *coeffsL1;
    *allpassL2.state = *coeffsL2;
    *allpassR1.state = *coeffsR1;
    *allpassR2.state = *coeffsR2;
    //*allpass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 300);
}

void AllPassbiquadAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AllPassbiquadAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AllPassbiquadAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto bufferSize = buffer.getNumSamples();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    bufferL.setSize(bufferSize, totalNumInputChannels);
    bufferR.setSize(bufferSize, totalNumInputChannels);
    bufferL.makeCopyOf(buffer);
    bufferR.makeCopyOf(buffer);
    
    juce::dsp::AudioBlock<float> blockL (bufferL);
    juce::dsp::ProcessContextReplacing<float> contextL (blockL);
    juce::dsp::AudioBlock<float> blockR (bufferR);
    juce::dsp::ProcessContextReplacing<float> contextR (blockL);
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    
    allpassL1.process(contextL);
    allpassL2.process(contextL);
    allpassR1.process(contextR);
    allpassR2.process(contextR);
    
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        auto c = cos.processSample(0);
        auto s = sin.processSample(0);
        auto sampleL = c * bufferL.getSample(0, i)  - s * bufferR.getSample(0, i) ;
        auto sampleR = c * bufferL.getSample(1, i)  - s * bufferR.getSample(1, i) ;
        buffer.setSample(0, i, sampleL);
        buffer.setSample(1, i, sampleR);
    }
 
}

//==============================================================================
bool AllPassbiquadAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AllPassbiquadAudioProcessor::createEditor()
{
    return new AllPassbiquadAudioProcessorEditor (*this);
}

//==============================================================================
void AllPassbiquadAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AllPassbiquadAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AllPassbiquadAudioProcessor();
}
