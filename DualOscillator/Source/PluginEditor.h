/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI.h"

//==============================================================================
/**
*/
class DualOscillatorAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    DualOscillatorAudioProcessorEditor (DualOscillatorAudioProcessor&);
    ~DualOscillatorAudioProcessorEditor() override;

    //==============================================================================
    void timerCallback () override
    {
        auto cFreq = 0;
        for (int i = 0 ; i < audioProcessor.synth.getNumVoices(); ++i)
        {
            if (auto voice = dynamic_cast<SynthVoice*>(audioProcessor.synth.getVoice(i)))
                {
                    auto& isOn = *audioProcessor.apvts.getRawParameterValue ("ONOFF");
                    if (isOn.load())
                        voice->setOn();
                    else
                        voice->setOff();
                    
                    cFreq = voice->getFreq();
                }
            ui.writeFreq(cFreq);
        }
    };
    
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    UI ui;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DualOscillatorAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DualOscillatorAudioProcessorEditor)
};
