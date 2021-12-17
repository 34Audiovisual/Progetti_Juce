/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GranulatorAudioProcessorEditor::GranulatorAudioProcessorEditor (GranulatorAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p), waveform(p)
{
    auto tapImage = juce::ImageCache::getFromMemory (BinaryData::grain_png, BinaryData::grain_pngSize);
            
            if (tapImage.isValid())
                logo.setImage (tapImage, juce::RectanglePlacement::stretchToFit);
            else
                jassertfalse;
    addAndMakeVisible(logo);
    addAndMakeVisible(link);
    link.setAlpha(0);
    link.setTooltip (TRANS("http://www.diegocapoccitti.com"));
    
    addAndMakeVisible(waveform);
    
    //position L
    positionSliderL.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    positionSliderL.setRange(0.f, 1.f);
    positionSliderL.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    positionSliderL.setTextValueSuffix(" %");
    positionSliderL.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    positionAttachmentL = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, NAME_POSITIONL, positionSliderL);
    addAndMakeVisible(positionSliderL);

    posLabelL.setFont (12.0f);
    posLabelL.setText ("Position L", juce::NotificationType::dontSendNotification);
    posLabelL.setJustificationType (juce::Justification::centredTop);
    addAndMakeVisible(posLabelL);
    
    //position R
    positionSliderR.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    positionSliderR.setRange(0.f, 1.f);
    positionSliderR.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    positionSliderR.setTextValueSuffix(" %");
    positionSliderR.setColour(juce::Slider::thumbColourId, juce::Colours::blue);
    positionAttachmentR = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, NAME_POSITIONR, positionSliderR);
    addAndMakeVisible(positionSliderR);

    posLabelR.setFont (12.0f);
    posLabelR.setText ("Position R", juce::NotificationType::dontSendNotification);
    posLabelR.setJustificationType (juce::Justification::centred);
    addAndMakeVisible(posLabelR);
    
    //grainLenght
    grainLenghtSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    grainLenghtSlider.setRange(MIN_GRAIN_LENGHT, MAX_GRAIN_LENGHT);
    grainLenghtSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    grainLenghtSlider.setTextValueSuffix(" ms");
    grainLenghtAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, NAME_GRAIN_LENGHT, grainLenghtSlider);
    addAndMakeVisible(grainLenghtSlider);

    lenLabel.setFont (12.0f);
    lenLabel.setText ("Grain Lenght", juce::NotificationType::dontSendNotification);
    lenLabel.setJustificationType (juce::Justification::centredTop);
    addAndMakeVisible(lenLabel);
    
    //spread
    spreadSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    spreadSlider.setRange(MIN_SPREAD, MAX_SPREAD);
    spreadSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    spreadSlider.setTextValueSuffix(" %");
    spreadAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, NAME_SPREAD, spreadSlider);
    addAndMakeVisible(spreadSlider);

    spreadLabel.setFont (12.0f);
    spreadLabel.setText ("L/R Spread", juce::NotificationType::dontSendNotification);
    spreadLabel.setJustificationType (juce::Justification::centredTop);
    addAndMakeVisible(spreadLabel);
    
    //grainPitch
    grainPitchSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    grainPitchSlider.setRange(MIN_GRAIN_PITCH, MAX_GRAIN_PITCH);
    grainPitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    grainPitchSlider.setTextValueSuffix(" %");
    grainPitchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, NAME_GRAIN_PITCH, grainPitchSlider);
    addAndMakeVisible(grainPitchSlider);
    
    pitchLabel.setFont (12.0f);
    pitchLabel.setText ("Grain Pitch", juce::NotificationType::dontSendNotification);
    pitchLabel.setJustificationType (juce::Justification::centredTop);
    addAndMakeVisible(pitchLabel);
    
    //on-off
    onOffButton.setButtonText("On/Off");
    onOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.parameters, NAME_ONOFF, onOffButton);
    addAndMakeVisible(onOffButton);
    
    //random
    randomButton.setButtonText("Randomize");
    randomAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.parameters, NAME_RANDOM, randomButton);
    addAndMakeVisible(randomButton);
    
    //link
    linkButton.setButtonText("link L/R");
    linkAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.parameters, NAME_LINK, linkButton);
    addAndMakeVisible(linkButton);
    
    //mono
    monoButton.setButtonText("mono");
    monoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.parameters, NAME_MONO, monoButton);
    addAndMakeVisible(monoButton);
    monoButton.onClick = [this]
    {
        if (monoButton.getToggleState())
        {
            linkButton.setEnabled(false);
            spreadLabel.setEnabled(false);
            spreadSlider.setEnabled(false);
        }
        else
        {
            linkButton.setEnabled(true);
            spreadLabel.setEnabled(true);
            spreadSlider.setEnabled(true);
        }
    };
    
    //voices
    voicesSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    voicesSlider.setRange(MIN_NUM_VOICES, MAX_NUM_VOICES);
    voicesSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    voicesSlider.setTextValueSuffix(" voice");
    voicesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, NAME_NUM_VOICES, voicesSlider);
    addAndMakeVisible(voicesSlider);
    
    voicesLabel.setFont (12.0f);
    voicesLabel.setText ("Num Voices", juce::NotificationType::dontSendNotification);
    voicesLabel.setJustificationType (juce::Justification::centredTop);
    addAndMakeVisible(voicesLabel);
    
    //deltaPos
    deltaPosSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    deltaPosSlider.setRange(MIN_DELTAPOS, MAX_DELTAPOS);
    deltaPosSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    deltaPosSlider.setTextValueSuffix(" %");
    deltaPosAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, NAME_DELTAPOS, deltaPosSlider);
    addAndMakeVisible(deltaPosSlider);
    
    deltaPosLabel.setFont (12.0f);
    deltaPosLabel.setText ("Pos Rand Var", juce::NotificationType::dontSendNotification);
    deltaPosLabel.setJustificationType (juce::Justification::centredTop);
    addAndMakeVisible(deltaPosLabel);
    
    //deltaLen
    deltaLenSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    deltaLenSlider.setRange(MIN_DELTALEN, MAX_DELTALEN);
    deltaLenSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    deltaLenSlider.setTextValueSuffix(" %");
    deltaLenAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, NAME_DELTALEN, deltaLenSlider);
    addAndMakeVisible(deltaLenSlider);
    
    deltaLenLabel.setFont (12.0f);
    deltaLenLabel.setText ("Len Rand Var", juce::NotificationType::dontSendNotification);
    deltaLenLabel.setJustificationType (juce::Justification::centredTop);
    addAndMakeVisible(deltaLenLabel);
    
    //deltaPitch
    deltaPitchSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    deltaPitchSlider.setRange(MIN_DELTAPITCH, MAX_DELTAPITCH);
    deltaPitchSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    deltaPitchSlider.setTextValueSuffix(" %");
    deltaPitchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, NAME_DELTAPITCH, deltaPitchSlider);
    addAndMakeVisible(deltaPitchSlider);
    
    deltaPitchLabel.setFont (12.0f);
    deltaPitchLabel.setText ("Pitch Rand Var", juce::NotificationType::dontSendNotification);
    deltaPitchLabel.setJustificationType (juce::Justification::centredTop);
    addAndMakeVisible(deltaPitchLabel);
    
    //play button
    addAndMakeVisible(playButton);
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    
    //monoPlugin
    if ( audioProcessor.getNumChannel() < 2)
    {
        audioProcessor.isPluginMono = true;
        audioProcessor.parameters.getParameterAsValue(NAME_MONO).setValue(true);
        monoButton.setEnabled(false);
        audioProcessor.parameters.getParameterAsValue(NAME_LINK).setValue(true);
        linkButton.setEnabled(false);
        audioProcessor.parameters.getParameterAsValue(NAME_SPREAD).setValue(0.f);
        spreadLabel.setEnabled(false);
        spreadSlider.setEnabled(false);
    }
    
    setSize (700, 420);
}

GranulatorAudioProcessorEditor::~GranulatorAudioProcessorEditor()
{
}

//==============================================================================
void GranulatorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
    
    if (*audioProcessor.parameters.getRawParameterValue(NAME_NUM_VOICES) == 1)
    {
        deltaLenSlider.setEnabled(false);
        deltaPitchSlider.setEnabled(false);
        deltaPosSlider.setEnabled(false);
    }
    else
    {
        deltaLenSlider.setEnabled(true);
        deltaPitchSlider.setEnabled(true);
        deltaPosSlider.setEnabled(true);
    }
    
    //separation Lines
    g.setColour(juce::Colours::white);
    g.drawLine(0, waveform.getBottom(), getWidth(), waveform.getBottom());
    g.drawLine(positionSliderR.getRight() + 1, waveform.getBottom(), positionSliderR.getRight() + 1, getHeight());
    g.drawLine(grainLenghtSlider.getRight() + 1, waveform.getBottom(), grainLenghtSlider.getRight() + 1, getHeight());
    g.drawLine(grainPitchSlider.getRight() + 1, waveform.getBottom(), grainPitchSlider.getRight() + 1, getHeight());
    g.drawLine(onOffButton.getX() - 10, waveform.getBottom(), onOffButton.getX() - 10, grainPitchSlider.getBottom() + 5);
    g.drawLine(grainPitchSlider.getRight() + 1, grainPitchSlider.getBottom() + 5, getWidth(), grainPitchSlider.getBottom() + 5);
    g.drawRect(getBounds());
    
    //Logo
    g.setFont(40);
    g.setColour(juce::Colours::skyblue);
    g.drawFittedText("GRAINdie", logo.getRight() + 30, 305, 110, 100, juce::Justification::centred, 1);
    g.setFont(12);
    g.drawFittedText("- audiogranulator plugin - ", logo.getRight()+ 30, 355, 110, 50, juce::Justification::centred, 1);
    g.setFont(10);
    g.setColour(juce::Colours::white);
    g.drawFittedText("by 34AudioVisual @2021", logo.getRight()+ 30, 369, 110, 50, juce::Justification::centred, 1);
    g.setColour(juce::Colours::white);
    g.drawFittedText("http//:www.diegocapoccitti.com/", logo.getRight() + 30, 379, 110, 50, juce::Justification::centred, 1);
    
    if (playButton.isDown())
        audioProcessor.isPlaying = true;
    else
        if (audioProcessor.isPlaying)
            audioProcessor.isPlaying = false;
}

void GranulatorAudioProcessorEditor::resized()
{
    int sliderDim = 100;
    int labelH = 15;
    waveform.setBounds(1, 1, getWidth()-2, 199);
    
    positionSliderL.setBounds(10, 220, sliderDim, sliderDim);
    posLabelL.setBounds(positionSliderL.getX(), positionSliderL.getY() - 8, positionSliderL.getWidth(), labelH);
    positionSliderR.setBounds(positionSliderL.getX()  + sliderDim, 220, sliderDim, sliderDim);
    posLabelR.setBounds(positionSliderR.getX(), positionSliderR.getY() - 8, positionSliderR.getWidth(), labelH);
    grainLenghtSlider.setBounds(positionSliderR.getX() + sliderDim + 2, positionSliderL.getY(), sliderDim, sliderDim);
    lenLabel.setBounds(grainLenghtSlider.getX(), grainLenghtSlider.getY() - 8, grainLenghtSlider.getWidth(), labelH);
    grainPitchSlider.setBounds(grainLenghtSlider.getX() + sliderDim, grainLenghtSlider.getY(), sliderDim, sliderDim);
    pitchLabel.setBounds(grainPitchSlider.getX(), grainPitchSlider.getY() - 8, grainPitchSlider.getWidth(), labelH);
    voicesSlider.setBounds(grainPitchSlider.getRight() + 40, grainPitchSlider.getY(), sliderDim - 5, sliderDim - 5);
    voicesLabel.setBounds(voicesSlider.getX(), voicesSlider.getY() - 8, voicesSlider.getWidth(), labelH);
    onOffButton.setBounds(getRight() - sliderDim, 220, sliderDim, 25);
    randomButton.setBounds(getRight() - sliderDim, 245, sliderDim, 25);
    playButton.setBounds(randomButton.getX(), randomButton.getBottom() + 10, randomButton.getWidth() - 10, 25);
    
    deltaPosSlider.setBounds(positionSliderL.getX() + sliderDim/2 + 10, positionSliderL.getBottom() + 15, sliderDim -20, sliderDim -20);
    deltaPosLabel.setBounds(deltaPosSlider.getX(), deltaPosSlider.getY() - 8, sliderDim - 20, labelH);
    spreadSlider.setBounds(positionSliderL.getX(), deltaPosSlider.getY(), 50, deltaPosSlider.getHeight());
    spreadLabel.setBounds(0, spreadSlider.getY() - 8, 70, labelH);
    
    monoButton.setBounds(deltaPosSlider.getRight() + 5, deltaPosSlider.getY() + 4, 55, 25);
    linkButton.setBounds(deltaPosSlider.getRight() + 5, monoButton.getY() + 30, 60, 25);
    
    deltaLenSlider.setBounds(grainLenghtSlider.getX() + 10, grainLenghtSlider.getBottom() + 15, sliderDim -20, sliderDim -20);
    deltaLenLabel.setBounds(grainLenghtSlider.getX(), deltaLenSlider.getY() - 8, sliderDim, labelH);
    
    deltaPitchSlider.setBounds(grainPitchSlider.getX() + 10, grainPitchSlider.getBottom() + 15, sliderDim -20, sliderDim -20);
    deltaPitchLabel.setBounds(grainPitchSlider.getX(), deltaPitchSlider.getY() - 8, sliderDim, labelH);
    
    logo.setBounds(grainPitchSlider.getRight() + 50, deltaPitchSlider.getY() +10, sliderDim -40, sliderDim - 40);
    
    link.setBounds( logo.getX(), logo.getY(), getWidth() - logo.getX(), getHeight() - logo.getY());
}
