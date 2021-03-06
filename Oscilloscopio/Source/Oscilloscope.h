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
     void pushBuffer(const float* data, int numSamples)
     {
         for (int i = 0; i < numSamples; ++i)
             buffer[++writePos % buffer.size()] = data[i];
         
         update();
         
     }
     //==========================================================================
     void paint(juce::Graphics& g) override
     {
         g.fillAll(juce::Colours::black.brighter(0.22f));
         juce::Rectangle<int> r = getLocalBounds();
         juce::Path path;
         path.startNewSubPath(0, 0.5 * r.getHeight());
         const float bufferYscale = 1.0f;
       
          int paintPos = 2;
         while (paintPos < buffer.size())
         {
             if (isZeroCrossing(paintPos))
                 break;
             ++paintPos;
         }
         if (paintPos == buffer.size())
             paintPos = 2;
         
         const int posOffset = paintPos;
         while (paintPos < buffer.size())
         {
             float x = (paintPos - posOffset) * r.getWidth() / paintSize;
             float y = 0.5 * ((bufferYscale *(- buffer[paintPos]) + 1) * r.getHeight());
             
             juce::Point<float> p;
             
             if (!(isnan(x) || isnan(y)))
                 p.setXY(x, y);
             
             path.lineTo(p);
             ++paintPos;
         }
         g.setColour(juce::Colour(0, 102, 204));
         g.strokePath(path ,juce::PathStrokeType(1.5f));
         ;
     }
 private:
     //==========================================================================
     void update()
     {
         repaint();
     }
     
     bool isZeroCrossing(int i) const noexcept
     {
         jassert(i > 0);
         return buffer[i] > buffer[i - 1] && buffer[i] > 0 && buffer[i - 1] < 0;
     }
     //==========================================================================
     std::array<float, 1024> buffer;
     std::size_t writePos;
    // const int bufferSize = 16384;
     const int paintSize = 256;
 };


 #endif  // OSCILLOSCOPE_H_INCLUDD

