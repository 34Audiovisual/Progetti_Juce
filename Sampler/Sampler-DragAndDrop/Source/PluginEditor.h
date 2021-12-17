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
class SamplerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::FileDragAndDropTarget
{
public:
    SamplerAudioProcessorEditor (SamplerAudioProcessor&);
    ~SamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //FileDragAndDropTarget pure virtual method
    bool isInterestedInFileDrag(const juce::StringArray& files) override
    {
        for (auto file : files)
        {
            if (file.contains (".wav") || file.contains (".mp3") || file.contains (".aif") )
            {
                repaint();
                return true;
            }
        }
        
        return false;
    }
    void filesDropped (const juce::StringArray& files, int x, int y) override
    {
        for (auto file : files)
        {
            if (isInterestedInFileDrag(file))
            {
                audioProcessor.loadFile(file);
                repaint();
            }
            
        }
    }
    

private:
    juce::TextButton mLoadButton { "Load " };
    SamplerAudioProcessor& audioProcessor;
    juce::Label loadedLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessorEditor)
};
