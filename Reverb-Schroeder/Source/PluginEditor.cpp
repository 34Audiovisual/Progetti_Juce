/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverbSchroederAudioProcessorEditor::ReverbSchroederAudioProcessorEditor (ReverbSchroederAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    decaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 30);
    decaySlider.setRange(0, 5000);
    decaySlider.setValue(100);
    addAndMakeVisible(decaySlider);
    decaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    decaySlider.onValueChange = [this]
    {
        audioProcessor.setDecay(decaySlider.getValue());
    };
    
    dryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 30);
    dryWetSlider.setRange(0, 1);
    dryWetSlider.setValue(0.5);
    addAndMakeVisible(dryWetSlider);
    dryWetSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRYWET", dryWetSlider);
    dryWetSlider.onValueChange = [this]
    {
        audioProcessor.setDryWet(dryWetSlider.getValue());
    };
    
    earlySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    earlySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 30);
    earlySlider.setRange(0, 1);
    earlySlider.setValue(0.5);
    addAndMakeVisible(earlySlider);
    earlySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "EARLY", earlySlider);
    earlySlider.onValueChange = [this]
    {
        audioProcessor.setEarly(earlySlider.getValue());
    };
    
    lowFreqSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    lowFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 30);
    lowFreqSlider.setRange(200, 15000);
    lowFreqSlider.setValue(7000);
    addAndMakeVisible(lowFreqSlider);
    lowSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LOWPASS", lowFreqSlider);
    lowFreqSlider.onValueChange = [this]
    {
        audioProcessor.setLowPass(lowFreqSlider.getValue());
    };
    
    highFreqSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    highFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 30);
    highFreqSlider.setRange(20, 500);
    highFreqSlider.setValue(30);
    addAndMakeVisible(highFreqSlider);
    highSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "HIGHPASS", highFreqSlider);
    highFreqSlider.onValueChange = [this]
    {
        audioProcessor.setHighPass(highFreqSlider.getValue());
    };
    
    
    setSize (400, 300);
}

ReverbSchroederAudioProcessorEditor::~ReverbSchroederAudioProcessorEditor()
{
}

//==============================================================================
void ReverbSchroederAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText("Decay", decaySlider.getX(), decaySlider.getY() - 15, decaySlider.getWidth(), 10, juce::Justification::centred, 1);
    g.drawFittedText("DryWet", dryWetSlider.getX(), dryWetSlider.getY() - 15, dryWetSlider.getWidth(), 10, juce::Justification::centred, 1);
    g.drawFittedText("EarlyRefl", earlySlider.getX(), earlySlider.getY() - 15, earlySlider.getWidth(), 10, juce::Justification::centred, 1);
    g.drawFittedText("HighCut", lowFreqSlider.getX(), lowFreqSlider.getY() - 10, lowFreqSlider.getWidth(), 10, juce::Justification::centred, 1);
    g.drawFittedText("LowCut", highFreqSlider.getX(), highFreqSlider.getY() - 10, highFreqSlider.getWidth(), 10, juce::Justification::centred, 1);
}

void ReverbSchroederAudioProcessorEditor::resized()
{
    decaySlider.setBounds(10, 20, 120, 120);
    dryWetSlider.setBounds(decaySlider.getRight(), 20, 120, 120);
    earlySlider.setBounds(dryWetSlider.getRight(), 20, 120, 120);
    highFreqSlider.setBounds(10, decaySlider.getBottom() +25, 120, 120);
    lowFreqSlider.setBounds(earlySlider.getX(), decaySlider.getBottom() +25, 120, 120);

}
