#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

struct Look : juce::LookAndFeel_V4
{

    Look() { }
    ~Look() { }
    
    void drawRotarySlider    (    juce::Graphics &     g,
                                                     int     x,
                                                     int     y,
                                                     int     width,
                                                     int     height,
                                                     float     sliderPosProportional,
                                                     float     rotaryStartAngle,
                                                     float     rotaryEndAngle,
                              juce::Slider &     slider ) override
    {

        juce::Image myStrip;

        myStrip = juce::ImageCache::getFromMemory (BinaryData::rot_png, BinaryData::rot_pngSize);

        /*
        const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() -      slider.getMinimum()); //value between 0 and 1 for current amount of rotation
        const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
        const int frameIdx = (int)ceil(fractRotation * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1

        const float radius = juce::jmin (width / 2.0f, height / 2.0f) ;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius - 1.0f;
        const float ry = centreY - radius /* - 1.0f*/;

//        g.drawImage( myStrip, // image
//                     (int)rx, (int)ry, myStrip.getWidth(), myStrip.getWidth(),   // dest
//                     0, frameIdx*myStrip.getWidth(), myStrip.getWidth(), myStrip.getWidth()); // source
        /*
        juce::AffineTransform rotator;


        auto tr = rotator.rotated ((float)sliderPosProportional* juce::MathConstants<float>::pi * 3.f / 2.f, (float)(myStrip.getWidth()/2), (float)(myStrip.getHeight()/2));
        
        DBG(rotaryEndAngle);
        
        g.drawImageTransformed(myStrip, tr);
        */
        const double fractRotation = (slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
            const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
            const int frameIdx = (int)ceil(fractRotation * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1

            const float radius = juce::jmin (width / 2.0f, height / 2.0f) ;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius - 1.0f;
            const float ry = centreY - radius - 1.0f;

            g.drawImage(myStrip,
                        (int)rx,
                        (int)ry,
                        2*(int)radius,
                        2*(int)radius,   //Dest
                        0,
                        frameIdx*myStrip.getWidth(),
                        myStrip.getWidth(),
                        myStrip.getWidth()); //Source
        

    }


    
};


class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Look look;
    juce::Slider slider;

    
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
