/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

class Rettangolo : public juce::Component
{
    void paint (juce::Graphics&) override;
    void resized() override;
};


class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback () override
    {
        update();
        resized();
        repaint();
    }
    
    void update()
    {
        float levelInDecibel = audioProcessor.livello;
        //DBG(levelInDecibel);
        level = juce::jmap  (levelInDecibel, -100.f, 0.f, 0.f, 200.f);
       // DBG(level);
        
    }

private:
    Rettangolo rett;
    float level = 200;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
