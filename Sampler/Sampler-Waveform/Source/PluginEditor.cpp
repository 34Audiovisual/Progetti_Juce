/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (SamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
//    mLoadButton.onClick = [&] { audioProcessor.loadFile();};
//    addAndMakeVisible(mLoadButton);
//
//    addAndMakeVisible(loadedLabel);
//    loadedLabel.setText("File Loaded", juce::dontSendNotification);
//    loadedLabel.setJustificationType(juce::Justification::centred);
//    loadedLabel.setVisible(false);
    
    setSize (600, 200);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
}

//==============================================================================
void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    
    if (mShouldBePainting)
    {
        g.setColour(juce::Colours::green);
        juce::Path p;
        mAudioPoints.clear();
        
        auto waveform = audioProcessor.getWaveForm();
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);
        
        //scale x axis
        for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }
        
        p.startNewSubPath(0, getHeight()/2);
        
        //scale y axis
        for (int sample = 0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float>(mAudioPoints[sample], -1.f, 1.f, 200, 0);
            p.lineTo(sample, point);
        }
        
        g.strokePath(p, juce::PathStrokeType(1));
        
        mShouldBePainting = false;
    };

//    if (audioProcessor.getNumSound() > 0)
//        loadedLabel.setVisible(true);
//    else
//        loadedLabel.setVisible(false);
}

void SamplerAudioProcessorEditor::resized()
{
    mLoadButton.setBounds(getBounds());
    loadedLabel.setBounds(0, getBottom() - 25, getWidth(), 25);
}
