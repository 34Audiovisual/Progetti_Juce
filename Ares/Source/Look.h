/*
  ==============================================================================

    Look.h
    Created: 15 Nov 2021 1:21:35am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct LookOnOff : juce::LookAndFeel_V4
{
    
    void drawToggleButton    (    juce::Graphics &     g,
                              juce::ToggleButton &     toggle,
                              bool     shouldDrawButtonAsHighlighted,
                              bool     shouldDrawButtonAsDown
                              )         override
    {

        bool isOn = toggle.getToggleState();
        juce::Image myStrip;

        myStrip = juce::ImageCache::getFromMemory (BinaryData::onOffButton_png, BinaryData::onOffButton_pngSize);
                
        if (isOn)
        {
            g.drawImage(myStrip, 0, 0, toggle.getWidth(), toggle.getHeight(), 0, 0, myStrip.getWidth(), myStrip.getHeight() / 2);
        }
        else
        {
            g.drawImage(myStrip, 0, 0, toggle.getWidth(), toggle.getHeight(), 0, myStrip.getHeight() / 2, myStrip.getWidth(), myStrip.getHeight() / 2);
        }
        
    }
};

struct LookSwitch : juce::LookAndFeel_V4
{
    
    void drawToggleButton    (    juce::Graphics &     g,
                              juce::ToggleButton &     toggle,
                              bool     shouldDrawButtonAsHighlighted,
                              bool     shouldDrawButtonAsDown
                              )         override
    {

        bool isOn = toggle.getToggleState();
        juce::Image myStrip , ledOn, ledOff;

        myStrip = juce::ImageCache::getFromMemory (BinaryData::switch_png, BinaryData::switch_pngSize);
        ledOn = juce::ImageCache::getFromMemory (BinaryData::analogLedOn_png, BinaryData::analogLedOn_pngSize);
        ledOff = juce::ImageCache::getFromMemory (BinaryData::analogLedOff_png, BinaryData::analogLedOff_pngSize);
                
        if (isOn)
        {
            g.drawImage(myStrip, 0, 0, toggle.getWidth(), toggle.getHeight(), 0, 0, myStrip.getWidth(), myStrip.getHeight() / 2);
            g.drawImage(ledOn, 0, 10, 20, 20, 0, 0, ledOn.getWidth(), ledOn.getHeight());
        }
        else
        {
            g.drawImage(myStrip, 0, 0, toggle.getWidth(), toggle.getHeight(), 0, myStrip.getHeight() / 2, myStrip.getWidth(), myStrip.getHeight() / 2);
            g.drawImage(ledOff, 0, 10, 20, 20, 0, 0, ledOff.getWidth(), ledOff.getHeight());
        }
        
    }
};

struct LookHorizSlider : juce::LookAndFeel_V4
{

    LookHorizSlider() { }
    ~LookHorizSlider() { }
    
    void drawLinearSlider    (    juce::Graphics &     g,
                              int     x,
                              int     y,
                              int     width,
                              int     height,
                              float     sliderPos,
                              float     minSliderPos,
                              float     maxSliderPos,
                              const juce::Slider::SliderStyle,
                              juce::Slider &slider
                              )     override
    {

        juce::Image myStrip, led;

        myStrip = juce::ImageCache::getFromMemory (BinaryData::horizSlider_png, BinaryData::horizSlider_pngSize);
        led = juce::ImageCache::getFromMemory (BinaryData::analogLed1_png, BinaryData::analogLed1_pngSize);
        
        const double fractPos = std::pow(((slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum())), 0.3); //value between 0 and 1 for current amount of rotation;
        
        //double fractPos = juce::NormalisableRange<double> { slider.getMinimum(), slider.getMaximum(), 0.1, 0.3, false }.convertTo0to1(fractPosPre);
        
        const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
        const int frameIdx = (int)ceil(fractPos * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1
        const int nFramesLed = led.getHeight()/led.getWidth(); // number of frames for vertical film strip
        const int frameIdxLed = (int)ceil(fractPos * ((double)nFramesLed-1.0) ); // current index from 0 --> nFrames-1

        g.drawImage(myStrip, x, y, width, height, 0, frameIdx*myStrip.getWidth(), myStrip.getWidth(), myStrip.getHeight()/128);
        g.drawImage(led, 160, 15, 20, 20, 0, frameIdxLed*led.getWidth(), led.getWidth(), led.getWidth());
    }
};

struct LookHorizSliderNorm : juce::LookAndFeel_V4
{

    LookHorizSliderNorm() { }
    ~LookHorizSliderNorm() { }
    
    void drawLinearSlider    (    juce::Graphics &     g,
                              int     x,
                              int     y,
                              int     width,
                              int     height,
                              float     sliderPos,
                              float     minSliderPos,
                              float     maxSliderPos,
                              const juce::Slider::SliderStyle,
                              juce::Slider &slider
                              )     override
    {

        juce::Image myStrip, led;

        myStrip = juce::ImageCache::getFromMemory (BinaryData::horizSlider_png, BinaryData::horizSlider_pngSize);
        led = juce::ImageCache::getFromMemory (BinaryData::analogLed1_png, BinaryData::analogLed1_pngSize);
        
        const double fractPos = ((slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum())); //value between 0 and 1 for current amount of rotation;
        
        const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
        const int frameIdx = (int)ceil(fractPos * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1
        const int nFramesLed = led.getHeight()/led.getWidth(); // number of frames for vertical film strip
        const int frameIdxLed = (int)ceil(fractPos * ((double)nFramesLed-1.0) ); // current index from 0 --> nFrames-1

        g.drawImage(myStrip, x, y, width, height, 0, frameIdx*myStrip.getWidth(), myStrip.getWidth(), myStrip.getHeight()/128);
        g.drawImage(led, 160, 15, 20, 20, 0, frameIdxLed*led.getWidth(), led.getWidth(), led.getWidth());
    }
};

struct LookKnobVol : juce::LookAndFeel_V4
{

    LookKnobVol() { }
    ~LookKnobVol() { }
    
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

        juce::Image myStrip, ledImage;

        myStrip = juce::ImageCache::getFromMemory (BinaryData::volumeKnob_png, BinaryData::volumeKnob_pngSize);
        
        ledImage =  juce::ImageCache::getFromMemory (BinaryData::analogLed1_png, BinaryData::analogLed1_pngSize);
        
        const double fractRotation = (slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
        const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
        const int frameIdx = (int)ceil(fractRotation * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1
        const int nFramesLed = ledImage.getHeight()/ledImage.getWidth(); // number of frames for vertical film strip
        const int frameIdxLed = (int)ceil(fractRotation * ((double)nFramesLed-1.0) ); // current index from 0 --> nFrames-1

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
        
        g.drawImage(ledImage, 110, 30, 20, 20, 0, frameIdxLed*ledImage.getWidth(), ledImage.getWidth(), ledImage.getWidth());
    }
};
    
struct LookKnobIdx : juce::LookAndFeel_V4
{

    LookKnobIdx() { }
    ~LookKnobIdx() { }
        
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

        juce::Image myStrip, led;

        myStrip = juce::ImageCache::getFromMemory (BinaryData::indexKnob_png, BinaryData::indexKnob_pngSize);
        led = juce::ImageCache::getFromMemory (BinaryData::led_png, BinaryData::led_pngSize);
            
        const double fractRotation = (slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
        const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
        const int frameIdx = (int)ceil(fractRotation * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1
        const int nFramesLed = led.getHeight()/led.getWidth(); // number of frames for vertical film strip
        const int frameIdxLed = (int)ceil(fractRotation * ((double)nFramesLed-1.0) ); // current index from 0 --> nFrames-1

        const float radius = juce::jmin (width / 2.0f, height / 2.0f) ;
            const float centreX = x + width * 0.5f;
            const float centreY = y + height * 0.5f;
            const float rx = centreX - radius - 1.0f;
                        const float ry = centreY - radius - 1.0f;
        
        g.drawImage(led, 0, 65, slider.getWidth(), 15, 0, frameIdxLed*led.getWidth(), 128, 32);
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
