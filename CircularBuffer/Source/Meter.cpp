/*
  ==============================================================================

    Meter.cpp
    Created: 2 Nov 2020 2:50:02pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#include "Meter.h"


Meter::Meter()
{
    level = -100.f;
}

Meter::~Meter() {}

void Meter::paint (juce::Graphics& g)
{
    //auto w = getWidth();
    auto h = getHeight();
    auto valNorm = juce::jmap  (level, NegInfDb, MaxDb, 0.f, 1.f);
    DBG(valNorm);
    
    g.setColour (juce::Colours::black);
    g.fillRect(bounds);
    g.setGradientFill(cg);
    juce::Rectangle<int> rect;
    
    rect = bounds.withHeight(h * valNorm).withY(h * (1.f - valNorm));
      
    g.fillRect(rect);
    g.setColour(juce::Colours::green);
    g.drawRect(rect, 2);
}
 
void Meter::update(float v)
{
    level = v;
    repaint();
    //DBG(level);
}

void Meter::resized()
{
    bounds = getLocalBounds ();
    h = bounds.getHeight();
    w = bounds.getWidth();
    x = bounds.getX();
    y = bounds.getY();
    float alpha = 0.5;
    auto colors = std::vector<juce::Colour>
    {
         juce::Colours::green.withAlpha(alpha),
         juce::Colours::green.withAlpha(alpha),
         juce::Colours::green.withAlpha(alpha),
         juce::Colours::yellow.withAlpha(alpha),
         juce::Colours::orange.withAlpha(alpha),
         juce::Colours::red.withAlpha(alpha)
     };
     for ( int i = 0; i < colors.size(); i++ )
     {
         cg.addColour(double(i) / double(colors.size() - 1) , colors[i] );
     }
}
