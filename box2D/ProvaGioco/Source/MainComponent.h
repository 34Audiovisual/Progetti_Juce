#pragma once

#include <JuceHeader.h>
#include "sprite.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component, public juce::Timer, public juce::KeyListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    
    void timerCallback () override
    {
        
        if (juce::KeyPress::isKeyCurrentlyDown (juce::KeyPress::rightKey))  
                astro.rotate(0.1f);
        
        if (juce::KeyPress::isKeyCurrentlyDown (juce::KeyPress::leftKey))
                astro.rotate(-0.1f);
        
        if (juce::KeyPress::isKeyCurrentlyDown ('a'))
        {
            float y = astro.getY();
            float x = astro.getX();
            float angle = astro.getAngle();
            //DBG(angle);
            float spost = 5.0f;
            float deltaY = spost * std::cos(angle);
            float deltaX = spost * std::sin(angle);
            
            juce::Rectangle<int> r;
            r.setBounds(x + deltaX, y - deltaY, astro.getWidth(), astro.getHeight());
            
            animator.animateComponent(&astro, r, 1.0, 30, false, 0.0, 0.0);
        }
        
        if (juce::KeyPress::isKeyCurrentlyDown (juce::KeyPress::downKey))
        {
            astro.setBounds(astro.getX(), astro.getY() + 2, astro.getWidth(), astro.getHeight());
        }
        
        if (juce::KeyPress::isKeyCurrentlyDown (juce::KeyPress::spaceKey))
        {

        }

        
    }
    void rotate(float vel);
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool keyPressed (const juce::KeyPress& key,
                             Component* originatingComponent) override
    {
        if (key.getTextCharacter() == 'a' ||
            key.getTextCharacter() == juce::KeyPress::leftKey ||
            key.getTextCharacter() == juce::KeyPress::rightKey ||
            key.getTextCharacter() == juce::KeyPress::downKey)
            return true;
            
        return false;
    }

private:
    
    Astro astro;
    juce::ComponentAnimator animator, bulletAnim;
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
