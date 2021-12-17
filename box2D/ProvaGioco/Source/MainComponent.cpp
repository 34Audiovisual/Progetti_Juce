#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setOpaque (true);
    setWantsKeyboardFocus (true);
    
    addAndMakeVisible(astro);
    //addKeyListener(this);
    startTimer(30);
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    stopTimer();
}
//==============================================================================
void MainComponent::rotate(float vel)
{
    astro.setAngle(vel);
    float angle = astro.getAngle();
    float x = astro.getX()  + astro.getWidth() / 2;
    float y = astro.getY() + astro.getHeight() / 2;
    astro.setTransform(juce::AffineTransform().rotation(angle, x, y));
}
//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    astro.setBounds(100, 100, 50, 50);
}
