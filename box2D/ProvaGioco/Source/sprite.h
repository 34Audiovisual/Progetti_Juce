/*
  ==============================================================================

    sprite.h
    Created: 24 Nov 2021 12:15:10am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct Astro : juce::Component
{
public:
    //==============================================================================
    Astro() {}
    ~Astro() override{}
    //==============================================================================
    void paint (juce::Graphics& g) override
    {
        g.addTransform(juce::AffineTransform::rotation(angle, getLocalBounds().getCentreX(), getLocalBounds().getCentreY()));
        g.fillAll (juce::Colours::transparentBlack);
        g.setColour (juce::Colours::white);
        int riduz = 7;
        g.drawLine(getWidth() / 2, 0, riduz, getHeight()-riduz);
        g.drawLine(riduz, getHeight()-riduz, getWidth() / 2, getHeight() / 2);
        g.drawLine(getWidth() / 2, getHeight() / 2, getWidth() - riduz, getHeight() - riduz);
        g.drawLine(getWidth() - riduz, getHeight() - riduz, getWidth() / 2, 0);
    }
    
    float getAngle() { return angle;}
    void setAngle(float a) { angle += a;}

    void resized() override
    {

    }
    
    void rotate(float vel)
    {
        setAngle(vel);
        repaint();
    }

private:
    float angle = 0.f;
};


struct Bullet : juce::Component
{
public:
    //==============================================================================
    Bullet() {}
    ~Bullet() override{}
    //==============================================================================
    void paint (juce::Graphics& g) override
    {
        g.fillRect(0, 0, 2, 2);
    }

    void resized() override
    {

    }
    
    void rotate(float vel)
    {
    }

private:
};
