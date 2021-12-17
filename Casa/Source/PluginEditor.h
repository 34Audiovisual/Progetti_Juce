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

struct Wall : juce::Component
{
    
    void paint (juce::Graphics& g) override
    {
       // g.fillAll (juce::Colours::brown);
        g.fillCheckerBoard(getLocalBounds().toFloat(), 10, 4, juce::Colours::sandybrown, juce::Colours::saddlebrown);
    }
};

struct Floor : juce::Component
{
    
    void paint (juce::Graphics& g) override
    {
        g.setColour(juce::Colours::green);
        g.drawLine(0.f, (float) getHeight() / 2.f, (float) getWidth(), (float) getHeight() / 2.f, 5.f);
    }
};

struct Roof : juce::Component
{
    
    void paint (juce::Graphics& g) override
    {
        g.setColour(juce::Colours::red);
        
        juce::Path roof;
        roof.addTriangle(0.f, (float) getHeight(), (float) getWidth(), (float) getHeight(), (float) getWidth() / 2.0f, 0.f);
        g.fillPath(roof);
    }
};

struct Scene : juce::Component
{
    Scene()
    {
        addAndMakeVisible(wall);
        addAndMakeVisible(floor);
        addAndMakeVisible(roof);
    }
    ~Scene()
    {
        
    }
    void paint (juce::Graphics&) override
    {
        
    }
    void resized() override
    {
        auto w = getWidth();
        auto h = getHeight();
        wall.setBounds(w - w / 2, h / 3, w / 4, w / 4);
        floor.setBounds(w / 40, wall.getBottom(), w - w / 20, h / 15);
        roof.setBounds(wall.getX(), wall.getY() - wall.getHeight() /  2 , wall.getWidth(), wall.getHeight() /  2);
    }
private:
    Wall wall;
    Floor floor;
    Roof roof;
};

class CasaAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CasaAudioProcessorEditor (CasaAudioProcessor&);
    ~CasaAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Scene scene;
    Scene scena2;
    CasaAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CasaAudioProcessorEditor)
};
