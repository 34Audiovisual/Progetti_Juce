/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AresAudioProcessorEditor::AresAudioProcessorEditor (AresAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //background
    setSize (600, 400);
    
    auto tapImage = juce::ImageCache::getFromMemory (BinaryData::Back_jpg, BinaryData::Back_jpgSize);
    background.setImage (tapImage, juce::RectanglePlacement::stretchToFit);
    addAndMakeVisible (background);
    
    title.setImage(juce::ImageCache::getFromMemory (BinaryData::rusty_png, BinaryData::rusty_pngSize));
    addAndMakeVisible(title);
    
    subtitle.setImage(juce::ImageCache::getFromMemory (BinaryData::padevolution_png, BinaryData::padevolution_pngSize));
    addAndMakeVisible(subtitle);
    
    company.setImage(juce::ImageCache::getFromMemory (BinaryData::audiovisual_png, BinaryData::audiovisual_pngSize));
    addAndMakeVisible(company);
    
    diegoc.setImage(juce::ImageCache::getFromMemory (BinaryData::diegocapoccitti_png, BinaryData::diegocapoccitti_pngSize));
    addAndMakeVisible(diegoc);
    
    site.setImage(juce::ImageCache::getFromMemory (BinaryData::site_png, BinaryData::site_pngSize));
    addAndMakeVisible(site);
    
    onOffLed.setImage(juce::ImageCache::getFromMemory (BinaryData::analogLedOff_png, BinaryData::analogLedOff_pngSize));
    addAndMakeVisible(onOffLed);

    //volume
    labelVolume.setImage(juce::ImageCache::getFromMemory (BinaryData::Volume1_png, BinaryData::Volume1_pngSize));
    addAndMakeVisible(labelVolume);
    
    addAndMakeVisible(volumeSlider);
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 20, 20);
    volumeSlider.setLookAndFeel(&lookVol);
    volumeSlider.setRange(0, 100);
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN" , volumeSlider);
    volumeSlider.onValueChange = [this] { takeVolumeText (); };
    takeVolumeText();
    gainText.setAlpha(0.6);
    gainText.setFont(12);
    gainText.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainText);
    
    //index
    labelIndex.setImage(juce::ImageCache::getFromMemory (BinaryData::Index_png, BinaryData::Index_pngSize));
    addAndMakeVisible(labelIndex);
    
    addAndMakeVisible(indexSlider);
    indexSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    indexSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 20, 20);
    indexSlider.setLookAndFeel(&lookIndex);
    indexSlider.setRange(0, 100);
    indexAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INDEX" , indexSlider);
    indexSlider.onValueChange = [this] { takeIndexText (); };
    takeIndexText();
    indexText.setAlpha(0.6);
    indexText.setFont(12);
    indexText.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(indexText);
    
    //time
    labelTime.setImage(juce::ImageCache::getFromMemory (BinaryData::time_png, BinaryData::time_pngSize));
    addAndMakeVisible(labelTime);
    
    addAndMakeVisible(timeSlider);
    timeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    timeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 20, 20);
    timeSlider.setLookAndFeel(&evolvehSlider);
    timeSlider.setRange(0, 100);
    timeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "EVOLVE" , timeSlider);
    timeSlider.onValueChange = [this] { takeTimeText (); };
    takeTimeText();
    timeText.setAlpha(0.6);
    timeText.setFont(12);
    addAndMakeVisible(timeText);
    
    //freq
    labelFreq.setImage(juce::ImageCache::getFromMemory (BinaryData::freq_png, BinaryData::freq_pngSize));
    addAndMakeVisible(labelFreq);
    
    addAndMakeVisible(freqSlider);
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    freqSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 20, 20);
    freqSlider.setLookAndFeel(&lookhSlider);
    freqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FREQ" , freqSlider);
    freqSlider.onValueChange = [this] { takeFreqText (); };
    takeFreqText();
    freqText.setAlpha(0.6);
    freqText.setFont(12);
    addAndMakeVisible(freqText);
    
    //reverb
    labelRev.setImage(juce::ImageCache::getFromMemory (BinaryData::reverb_png, BinaryData::reverb_pngSize));
    addAndMakeVisible(labelRev);
    revButton.setLookAndFeel(&lookSwitch);
    addAndMakeVisible(revButton);
    revAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "REVERB" , revButton);
    
    //random
    labelRand.setImage(juce::ImageCache::getFromMemory (BinaryData::random_png, BinaryData::random_pngSize));
    addAndMakeVisible(labelRand);
    randButton.setLookAndFeel(&lookSwitch);
    addAndMakeVisible(randButton);
    randAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "RANDOM" , randButton);
    randButton.onClick = [this] { takeTimeText (); };

    //onOff
    labelOnOff.setImage(juce::ImageCache::getFromMemory (BinaryData::OnOff_png, BinaryData::OnOff_pngSize));
    addAndMakeVisible(labelOnOff);
    onOffButton.setLookAndFeel(&lookOn);
    onOffButton.onClick = [this]
    {
        if (onOffButton.getToggleState())
        onOffLed.setImage(juce::ImageCache::getFromMemory (BinaryData::analogLedOn_png, BinaryData::analogLedOn_pngSize));
        else
        onOffLed.setImage(juce::ImageCache::getFromMemory (BinaryData::analogLedOff_png, BinaryData::analogLedOff_pngSize));
    };
    addAndMakeVisible(onOffButton);
    onAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "ONOFF" , onOffButton);
    
    //link website
    addAndMakeVisible (hyperlinkButton);
    hyperlinkButton.setTooltip (TRANS("http://www.diegocapoccitti.com"));
    hyperlinkButton.setAlpha(0);
}

AresAudioProcessorEditor::~AresAudioProcessorEditor()
{
}

//==============================================================================
void AresAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void AresAudioProcessorEditor::resized()
{
    //background
    background.setBounds(getBounds());
    title.setBounds(getWidth() / 2 - 200, 0, 400, 150);
    subtitle.setBounds(title.getX() + 50, title.getBottom() - 75, title.getWidth(), 60);
    company.setBounds(-20, getBottom() - 50, 150, 50);
    diegoc.setBounds(company.getRight() - 20, company.getY() -20, 140, 90);
    site.setBounds(diegoc.getRight() + 20, company.getY() -20, 150, 90);
    hyperlinkButton.setBounds (site.getBounds());
    
    //volume
    labelVolume.setBounds(430, 220, 150, 110);
    volumeSlider.setBounds(labelVolume.getX() + 15, labelVolume.getBottom() - 70, labelVolume.getWidth(), labelVolume.getWidth() );
    gainText.setBounds(volumeSlider.getRight() -70, volumeSlider.getBottom() - 50, 100, 20);
    
    //time
    labelTime.setBounds(0, 230, 200, 60);
    timeSlider.setBounds(20, 270, 180, 40);
    timeText.setBounds(timeSlider.getRight() - 40, timeSlider.getBottom(), 150, 20);
    
    //freq
    labelFreq.setBounds(40, 140, 150, 50);
    freqSlider.setBounds(20, 175, 180, 40);
    freqText.setBounds(freqSlider.getRight() - 40, freqSlider.getBottom(), 100, 20);
    
    //reverb
    labelRev.setBounds(getWidth() / 2 + 120, 125, 100, 80);
    revButton.setBounds(labelRev.getX() + 25, labelRev.getBottom() - 35, 50, 50);
    
    //random
    labelRand.setBounds(labelRev.getRight() - 10, 140, 100, 50);
    randButton.setBounds(labelRand.getX() + 15, labelRev.getBottom() - 35, 50, 50);
    
    //index
    labelIndex.setBounds(getWidth() / 2 - 50, 150, 100, 60);
    indexSlider.setBounds(labelIndex.getX() + 15, labelIndex.getBottom() - 20, 80, 80);
    indexText.setBounds(indexSlider.getX(), indexSlider.getBottom(), indexSlider.getWidth(), 20);
    
    //onOff
    labelOnOff.setBounds(60, 20, 70, 70);
    onOffLed.setBounds(450, 60, 20, 20);
    onOffButton.setBounds(labelOnOff.getX() + 10, labelOnOff.getBottom() - 35, 50, 50);
    
}
