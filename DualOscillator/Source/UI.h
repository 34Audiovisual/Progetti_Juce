/*
  ==============================================================================

    UI.h
    Created: 1 Nov 2021 2:39:04am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
struct LookOn : juce::LookAndFeel_V4
{
    void drawToggleButton (juce::Graphics & g, juce::ToggleButton & toggle, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        bool isOn = toggle.getToggleState();
        juce::Image onImage, offImage;
        onImage = juce::ImageCache::getFromMemory (BinaryData::switch_2_png, BinaryData::switch_2_pngSize);
        
        offImage = juce::ImageCache::getFromMemory (BinaryData::switch_1_png, BinaryData::switch_1_pngSize);
        
        if (isOn)
        {
            g.drawImage(onImage, 0, 0, toggle.getWidth(), toggle.getHeight(), 0, 0, 110, 115);
        }
        else
        {
            g.drawImage(offImage, 0, 0, toggle.getWidth(), toggle.getHeight(), 0, 0, 110, 115);
        }
    }
};

struct LookKnobPicc : juce::LookAndFeel_V4
{

    LookKnobPicc() { }
    ~LookKnobPicc() { }
    
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

        myStrip = juce::ImageCache::getFromMemory (BinaryData::knobb_png, BinaryData::knobb_pngSize);
        
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
 
struct LookKnobBig : juce::LookAndFeel_V4
{

    LookKnobBig() { }
    ~LookKnobBig() { }
    
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

        juce::Image knob, corona;
        
        corona = juce::ImageCache::getFromMemory (BinaryData::corona_png, BinaryData::corona_pngSize);

        g.drawImage(corona, 0, 0, slider.getWidth(), slider.getHeight(), 0, 0, corona.getWidth(), corona.getHeight());
        
        knob = juce::ImageCache::getFromMemory (BinaryData::knobb_png, BinaryData::knobb_pngSize);
        
        const double fractRotation = (slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation


        const float radius = juce::jmin (width / 2.0f, height / 2.0f) ;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;

        auto xEndLine = centreX - 0.7f * radius *  std::cos(fractRotation * juce::MathConstants<float>::pi * 1.3f - juce::MathConstants<float>::pi * 0.15f);
        auto yEndLine = centreY - 0.7f * radius * std::sin( fractRotation * juce::MathConstants<float>::pi * 1.3f - juce::MathConstants<float>::pi * 0.15f);
        
        g.setColour(juce::Colours::red);

        g.drawLine(centreX, centreY, xEndLine, yEndLine, 2);
    }
};


struct LookKnobCentr : juce::LookAndFeel_V4
{

    LookKnobCentr() { }
    ~LookKnobCentr() { }
    
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

        juce::Image knob, corona;
        
        corona = juce::ImageCache::getFromMemory (BinaryData::border_png, BinaryData::border_pngSize);

        g.drawImage(corona, 0, 0, slider.getWidth(), slider.getHeight(), 0, 0, corona.getWidth(), corona.getHeight());
        
        knob = juce::ImageCache::getFromMemory (BinaryData::knobCentr_png, BinaryData::knobCentr_pngSize);
        
        const double fractRotation = (slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
        const int nFrames = knob.getHeight()/knob.getWidth(); // number of frames for vertical film strip
         int frameIdx = (int)ceil(fractRotation * ((double)nFrames * 20 -1.0) ); // current index from 0 --> nFrames-1
        frameIdx %= nFrames;

        const float radius = juce::jmin (width / 2.0f, height / 2.0f) ;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius*0.7 - 1.0f;
        const float ry = centreY - radius*0.7 - 1.0f;

        g.drawImage(knob,
                    (int)rx,
                    (int)ry,
                    2*(int)(radius*0.7),
                    2*(int)(radius*0.7),   //Dest
                    0,
                    frameIdx*knob.getWidth(),
                    knob.getWidth(),
                    knob.getWidth()); //Source
    }

};

class UI  : public juce::Component
{
public:
    UI(juce::AudioProcessorValueTreeState& apvts);
    ~UI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setKnob(juce::Slider& slider);
    void writeFreq (float f)
    {
        juce::String testo = juce::String(f).toDecimalStringWithSignificantFigures(f, 5) + " Hz";
        freqLabel.setText(testo, juce::dontSendNotification);
        repaint();
        
    };

private:
    LookKnobPicc lookPicc;
    LookOn lookOn;
    LookKnobBig lookCorona;
    LookKnobCentr lookCentr;
    
    juce::Slider modFreqSlider, freqDevSlider, dutySlider, outputSlider;
    juce::Slider waveformSel, waveformAux, dirtySel, dirtyAux;
    juce::ComboBox dirtyChoice, waveChoice;
    juce::Slider coronaKnobSlider, centrKnobSlider;
    juce::ImageComponent background;
    
    juce::ToggleButton on;
    juce::ImageComponent ledOn;
    
    juce::Label freqLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> onAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> coronaAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> centrKnobAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> dirtyChoiceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveChoiceAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UI)
};
