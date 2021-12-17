/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"

//==============================================================================
/**
*/
class SynthClassAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthClassAudioProcessorEditor (SynthClassAudioProcessor&);
    ~SynthClassAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::ComboBox oscSelector;
    
    SynthClassAudioProcessor& audioProcessor;
    AdsrComponent adsr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelAttachment;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthClassAudioProcessorEditor)
};
