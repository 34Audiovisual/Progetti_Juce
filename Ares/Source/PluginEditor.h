/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Look.h"

//==============================================================================
/**
*/


class AresAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AresAudioProcessorEditor (AresAudioProcessor&);
    ~AresAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    LookKnobVol lookVol;
    LookHorizSlider lookhSlider;
    LookHorizSliderNorm evolvehSlider;
    LookSwitch lookSwitch;
    LookKnobIdx lookIndex;
    LookOnOff lookOn;
    
    juce::ImageComponent background, onOffLed;
    juce::ImageComponent title, subtitle, company, diegoc, site;
    juce::ImageComponent labelVolume, labelRev, labelRand, labelTime, labelFreq, labelIndex, labelOnOff;
    
    juce::Slider volumeSlider, indexSlider, timeSlider, freqSlider;
    juce::ToggleButton revButton, randButton, onOffButton;
    juce::Label freqText, indexText, gainText, timeText;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> onAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> revAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> randAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> indexAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> timeAttachment;
    
    void takeFreqText ()
    {
        juce::String testo = static_cast<juce::String>(audioProcessor.getFreq()) + " Hz";
        freqText.setText(testo, juce::dontSendNotification);
    }
    
    void takeTimeText ()
    {
        juce::String testo;
        
        if ( ! audioProcessor.getRandom())
            testo = static_cast<juce::String>(audioProcessor.getTime()) + " Sec";
        else
        {
            testo = "Random: 0 - " + static_cast<juce::String>(audioProcessor.getTime()) + " Sec";
        }
        
        timeText.setText(testo, juce::dontSendNotification);
    }
    
    void takeIndexText ()
    {
        juce::String testo = static_cast<juce::String>(audioProcessor.getIndex()) + "";
        indexText.setText(testo, juce::dontSendNotification);
    }
    
    void takeVolumeText ()
    {
        juce::String testo = juce::String::toDecimalStringWithSignificantFigures(audioProcessor.getGain(), 1);
        gainText.setText(testo, juce::dontSendNotification);
    }
    
    juce::HyperlinkButton hyperlinkButton {"www.diegocapoccitti.com", juce::URL ("http://www.diegocapoccitti.com")};
    
    AresAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AresAudioProcessorEditor)
};
