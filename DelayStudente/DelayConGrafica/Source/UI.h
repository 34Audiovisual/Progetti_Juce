/*
  ==============================================================================

    UI.h
    Created: 16 Nov 2021 4:01:56pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginParameters.h"
#include "MyLookAndFeel.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UI  : public juce::Component
{
public:
    //==============================================================================
    UI ( juce::AudioProcessorValueTreeState& apvts );
    ~UI() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;



private:
    //juce::AudioProcessorValueTreeState parameters;
    //[UserVariables]   -- You can add your own custom variables in this section.

    MyLookAndFeel myTheme;

    std::unique_ptr<SliderAttachment> timeAttachment;
    std::unique_ptr<SliderAttachment> timeLAttachment;
    std::unique_ptr<SliderAttachment> timeRAttachment;
    std::unique_ptr<SliderAttachment> fbAttachment;
    std::unique_ptr<SliderAttachment> dwAttachment;
    std::unique_ptr<ButtonAttachment> slrAttachment;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> timeSlider;
    std::unique_ptr<juce::Slider> timeSliderL;
    std::unique_ptr<juce::Slider> timeSliderR;
    std::unique_ptr<juce::Slider> fbSlider;
    std::unique_ptr<juce::Slider> dwSlider;
    std::unique_ptr<juce::ToggleButton> slrButton;


    //==============================================================================
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
