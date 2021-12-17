/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ImageAudioProcessorEditor::ImageAudioProcessorEditor (ImageAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    auto tapImage = juce::ImageCache::getFromMemory (BinaryData::tapLogo_png, BinaryData::tapLogo_pngSize);
        
        if (tapImage.isValid())
            logo.setImage (tapImage, juce::RectanglePlacement::stretchToFit);
        else
            jassertfalse;
    addAndMakeVisible (logo);
    setSize (400, 300);
}

ImageAudioProcessorEditor::~ImageAudioProcessorEditor()
{
}

//==============================================================================
void ImageAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void ImageAudioProcessorEditor::resized()
{
    logo.setBounds(10, 10, 100, 100);
}
