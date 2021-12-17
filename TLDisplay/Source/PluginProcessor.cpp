/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ControlerMidiAudioProcessor::ControlerMidiAudioProcessor()
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
    valuesTreeMidi.setProperty("displayValue", 0, nullptr);
    valuesTreeMidi.setProperty("positionValue", 0, nullptr);
    valuesTreeMidi.setProperty("riga1Value", "", nullptr);
    valuesTreeMidi.setProperty("riga2Value", "", nullptr);
    valuesTreeMidi.setProperty("sentValue", 0, nullptr);
    
    auto midiDevice = juce::MidiOutput::getAvailableDevices();

    for ( auto m : midiDevice)
    {
        //DEBUG
        juce::String dev = "Device name: " + m.name + " Device ID: " + m.identifier;
        DBG(dev);
        //DEBUG

       // if (m.name == "TLLINK")
        //if (m.name == "MIDI Monitor (Untitled)")
        if (m.name == "TLLINK")
            midiDev = juce::MidiOutput::openDevice(m.identifier);
    }
    
    startTimer(30);
}

ControlerMidiAudioProcessor::~ControlerMidiAudioProcessor()
{
    stopTimer();
}

//==============================================================================
const juce::String ControlerMidiAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ControlerMidiAudioProcessor::acceptsMidi() const
{
    return false;
}

bool ControlerMidiAudioProcessor::producesMidi() const
{
    return true;
}

bool ControlerMidiAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ControlerMidiAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ControlerMidiAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ControlerMidiAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ControlerMidiAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ControlerMidiAudioProcessor::getProgramName (int index)
{
    return {};
}

void ControlerMidiAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ControlerMidiAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}

void ControlerMidiAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ControlerMidiAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ControlerMidiAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

}

//==============================================================================
bool ControlerMidiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ControlerMidiAudioProcessor::createEditor()
{
    return new ControlerMidiAudioProcessorEditor (*this);
}


void ControlerMidiAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, false);
    DBG( "WRITE : " << valuesTreeMidi.toXmlString() );
    valuesTreeMidi.writeToStream(mos);
     
}

void ControlerMidiAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
        DBG( "READ : " << tree.toXmlString() );
        if( tree.isValid() )
        {
            valuesTreeMidi = tree;
        }
    //retrig();
    
    int d = (int)((valuesTreeMidi.getPropertyAsValue("displayValue", nullptr, true)).getValue());
    int p = (int)((valuesTreeMidi.getPropertyAsValue("positionValue", nullptr, true)).getValue());
   // int stepTimeMs = 200;
    //auto delayTimeMs = stepTimeMs * (d + p);
    auto delayTimeMs = 4000 + (d * 1000 - 1000 + p * 100);
    std::function<void()> trig = [this] {retrig();};
    juce::Timer::callAfterDelay(delayTimeMs, trig);
}
//==============================================================================
void ControlerMidiAudioProcessor::retrig ()
{
    auto started = valuesTreeMidi.getPropertyAsValue("sentValue", nullptr, true);
    
    if ((bool)(started.getValue()))
    {
        int display = (int)((valuesTreeMidi.getPropertyAsValue("displayValue", nullptr, true)).getValue());
        int position = (int)((valuesTreeMidi.getPropertyAsValue("positionValue", nullptr, true)).getValue());
        juce::String riga1 = (valuesTreeMidi.getPropertyAsValue("riga1Value", nullptr, true)).toString();
        juce::String riga2 = (valuesTreeMidi.getPropertyAsValue("riga2Value", nullptr, true)).toString();
        auto array1 = makeNoteArray(riga1);
        auto array2 = makeNoteArray(riga2);
        addEvent(display, position - 1, array1, array2);
    };
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ControlerMidiAudioProcessor();
}


