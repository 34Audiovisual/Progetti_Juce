/*
  =============================================================================
    Oscilloscope
    Created: 19 Nov 2021 7:38:59
    Author:  Diego Capocciti
  =============================================================================
*/
#pragma on
#include <JuceHeader.h>

#ifndef OSCILLOSCOPE_H_INCLUDED
#define OSCILLOSCOPE_H_INCLUDD

 #include <array>
class Oscilloscope : public juce::Component
 {
 public:
     //==========================================================================
     Oscilloscope()
         : writePos(0)
     {
        // startTimer(60);
     }
     //==========================================================================
     void pushBuffer(juce::AudioBuffer<float>& b)
     {
         buffer.makeCopyOf(b);
         
         update();
         
     }
     //==========================================================================
     void paint(juce::Graphics& g) override
     {
         g.fillAll(juce::Colours::black.brighter(0.22f));
         juce::Rectangle<int> r = getLocalBounds();
         int w = r.getWidth();
         int h = r.getHeight();
         juce::Path pathH;
         //float a = buffer.getSample(1, 1) * w * 0.5 + w * 0.5;
         //float y0 = buffer.getSample(1, 0) * h * 0.5 + h * 0.5;
         //pathH.startNewSubPath( x0, y0);
         const float bufferYscale = 1.0f;


         for (int i = 0; i < buffer.getNumSamples(); ++i)
         {
             float x = buffer.getSample(0, i) * w * 0.5 + w * 0.5 ;
             float y = buffer.getSample(1, i) * h * 0.5 + h * 0.5;
             
             if (i == 0)
             {
                 pathH.startNewSubPath( x, y);
             }
             
             else
             {
             juce::Point<float> p;
             
             if (!(isnan(x) || isnan(y)))
                 p.setXY(x, y);
             
             pathH.lineTo(p);
                 
             }
         }
         g.setColour(juce::Colour(0, 102, 204));
         g.strokePath(pathH ,juce::PathStrokeType(2.f));
         ;
     }
 private:
     //==========================================================================
     juce::AudioBuffer<float> buffer;
     void update()
     {
         repaint();
     }
     
     //==========================================================================
     std::array<float, 1024> buffer1, buffer2;
     std::size_t writePos;

     const int paintSize = 256;
 };


 #endif  // OSCILLOSCOPE_H_INCLUDD

