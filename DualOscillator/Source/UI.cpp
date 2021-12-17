/*
  ==============================================================================

    UI.cpp
    Created: 1 Nov 2021 2:39:04am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#include <JuceHeader.h>
#include "UI.h"

//==============================================================================
UI::UI(juce::AudioProcessorValueTreeState& apvts)
{
    //Background
    auto backImage = juce::ImageCache::getFromMemory (BinaryData::back_png, BinaryData::back_pngSize);
            
    if (backImage.isValid())
        background.setImage (backImage, juce::RectanglePlacement::stretchToFit);
    else
        jassertfalse;
    addAndMakeVisible (background);
    
    //Knobs
    setKnob(modFreqSlider);
    setKnob(freqDevSlider);
    setKnob(dutySlider);
    setKnob(outputSlider);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "OUTPUT" , outputSlider);
    
    addAndMakeVisible(waveChoice);
    waveChoice.setVisible(false);
    waveChoice.addItem("Sine", 1);
    waveChoice.addItem("Saw", 2);
    waveChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "WAVEFORM" , waveChoice);
    setKnob(waveformSel);
    waveformSel.setValue(25);
    addAndMakeVisible(waveformAux);
    waveformAux.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    waveformAux.setTextBoxStyle(juce::Slider::NoTextBox, true, 10, 10);
    waveformAux.setRange(0, 100);
    waveformAux.onValueChange = [this]
    {
        if (waveformAux.getValue() < 50)
        {
            waveformSel.setValue(25);
            waveChoice.setSelectedId(1);
        }
        else
        {
            waveformSel.setValue(35);
            waveChoice.setSelectedId(2);
        }
    };
    waveformAux.setAlpha(0);
    
    addAndMakeVisible(dirtyChoice);
    dirtyChoice.setVisible(false);
    dirtyChoice.addItem("0.01V", 1);
    dirtyChoice.addItem("0.1V", 2);
    dirtyChoice.addItem("1V", 3);
    dirtyChoice.addItem("10V", 4);
    dirtyChoice.setSelectedId(3);
    dirtyChoiceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, "DIRTY" , dirtyChoice);
    setKnob(dirtySel);
    dirtySel.setValue(50);
    addAndMakeVisible(dirtyAux);
    dirtyAux.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    dirtyAux.setTextBoxStyle(juce::Slider::NoTextBox, true, 10, 10);
    dirtyAux.setRange(0, 100);
    dirtyAux.onValueChange = [this]
    {
        if (dirtyAux.getValue() <= 25)
        {
            dirtySel.setValue(30);
            dirtyChoice.setSelectedId(1);
        }
        if (dirtyAux.getValue() > 25 && dirtyAux.getValue() <= 50)
        {
            dirtySel.setValue(40);
            dirtyChoice.setSelectedId(2);
        }
        if (dirtyAux.getValue() > 50 && dirtyAux.getValue() <= 75)
        {
            dirtySel.setValue(50);
            dirtyChoice.setSelectedId(3);
        }
        if (dirtyAux.getValue() > 75 && dirtyAux.getValue() <= 100)
        {
            dirtySel.setValue(60);
            dirtyChoice.setSelectedId(4);
        }
        
    };
    dirtyAux.setAlpha(0);
    
    setKnob(coronaKnobSlider);
    coronaKnobSlider.setLookAndFeel(&lookCorona);
    coronaAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "FREQ" , coronaKnobSlider);
    
    setKnob(centrKnobSlider);
    centrKnobSlider.setLookAndFeel(&lookCentr);
    centrKnobSlider.setMouseDragSensitivity(3000);
    centrKnobAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "FREQFINE" , centrKnobSlider);
    //switches
    on.setLookAndFeel(&lookOn);

    on.onClick = [this]
    {
        if (on.getToggleState())
        {
            ledOn.setImage (juce::ImageCache::getFromMemory (BinaryData::big_led_2_png, BinaryData::big_led_2_pngSize));
            repaint();
        }
        else
        {
            ledOn.setImage (juce::ImageCache::getFromMemory (BinaryData::big_led_1_png, BinaryData::big_led_1_pngSize));
            repaint();
        }
    };
    addAndMakeVisible(on);
    on.setToggleState(false , juce::dontSendNotification);
    
    onAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "ONOFF" , on);
    
    ledOn.setImage (juce::ImageCache::getFromMemory (BinaryData::big_led_1_png, BinaryData::big_led_1_pngSize));
    addAndMakeVisible(ledOn);
    
    //Labels
    addAndMakeVisible(freqLabel);
    freqLabel.setText("Hz", juce::dontSendNotification);
    freqLabel.setJustificationType(juce::Justification::centred);
    
}

UI::~UI()
{

}

void UI::paint (juce::Graphics& g)
{
    
   // g.drawImage(ledOn, 0, 0, 70, 70, 0, 0, 110, 115);
}

void UI::resized()
{
    auto bottomY = getBottom() - getHeight() * 0.125;
    auto firstX = getWidth() * 0.12;
    auto wKnob = 50;

    background.setBounds(getBounds());
    
    modFreqSlider.setBounds(firstX, bottomY, wKnob, wKnob);
    freqDevSlider.setBounds(modFreqSlider.getRight() + 70, bottomY, wKnob, wKnob);
    dutySlider.setBounds(freqDevSlider.getRight() + 68, bottomY, wKnob, wKnob);
    outputSlider.setBounds(dutySlider.getRight() + 64, bottomY, wKnob, wKnob);
    
    coronaKnobSlider.setBounds(95, 190, 340, 340);
    centrKnobSlider.setBounds(185, 280, 160, 160);
    
    waveformSel.setBounds(firstX, 210, wKnob, wKnob);
    waveformAux.setBounds(firstX, 210, wKnob, wKnob);
    dirtySel.setBounds(outputSlider.getX() + 15, 210, wKnob, wKnob);
    dirtyAux.setBounds(outputSlider.getX() + 15, 160, wKnob, wKnob + 50);
    
    on.setBounds(62, 115, 50, 50);
    
    ledOn.setBounds(getWidth() / 2 - 12, 5, 40, 40);
    
    freqLabel.setBounds (0, coronaKnobSlider.getBottom(), getWidth(), 20);
    
}

void UI::setKnob(juce::Slider& slider)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 20, 20);
    slider.setLookAndFeel(&lookPicc);
    slider.setRange(0, 100);
    addAndMakeVisible(slider);
}
