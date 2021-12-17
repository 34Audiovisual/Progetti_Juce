/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Oscilloscope.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class LissajousAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer
{
public:
    LissajousAudioProcessorEditor (LissajousAudioProcessor&);
    ~LissajousAudioProcessorEditor() override;

    void timerCallback() override
    {
        auto buff = audioProcessor.getBuffer();
        
        scope.pushBuffer(buff);

    }
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    LissajousAudioProcessor& audioProcessor;
    Oscilloscope scope;
    
    juce::Slider ampXSlider, freqXSlider, multXSlider, phaseXSlider;
    juce::Slider ampYSlider, freqYSlider, multYSlider, phaseYSlider;
    juce::Slider dimSlider, linkedSlider, velSlider;
    juce::Slider offXSlider, offYSlider;
    
    std::unique_ptr<SliderAttachment> ampXAttachment;
    std::unique_ptr<SliderAttachment> freqXAttachment;
    std::unique_ptr<SliderAttachment> multXAttachment;
    std::unique_ptr<SliderAttachment> phaseXAttachment;
    std::unique_ptr<SliderAttachment> ampYAttachment;
    std::unique_ptr<SliderAttachment> freqYAttachment;
    std::unique_ptr<SliderAttachment> multYAttachment;
    std::unique_ptr<SliderAttachment> phaseYAttachment;
    std::unique_ptr<SliderAttachment> dimAttachment;
    std::unique_ptr<SliderAttachment> offxAttachment;
    std::unique_ptr<SliderAttachment> offyAttachment;
    
    void makeSlider (juce::Slider& slider)
    {
        int boxW = 50;
        int boxH = 20;
        slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, boxW, boxH);
        slider.setNumDecimalPlacesToDisplay(1);
        addAndMakeVisible(slider);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LissajousAudioProcessorEditor)
};
