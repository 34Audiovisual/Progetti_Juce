/*
  ==============================================================================

    Delay.cpp
    Created: 27 Sep 2021 2:19:59am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#include "Flanger.h"

void Flanger::getFromBiggerBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& biggerBuffer, int writePosition, int channel)
{
   
    int biggerBufferSize = biggerBuffer.getNumSamples();
    int bufferSize = buffer.getNumSamples();
    int delayMinInSamples = (int) (sampleRate * delayMinInMs / 1000);
    
    auto currentSample = (float) std::sin (currentAngle);
    currentAngle += angleDelta;
    auto widthInSamples = sampleRate * (widthInMs / 1000);
    auto currentWidth = widthInSamples * ( currentSample + 1.f )/2.f;
    
    auto* biggerBufferData = biggerBuffer.getWritePointer (channel);
    const int readPosition = static_cast<int> (biggerBufferSize + writePosition - currentWidth - delayMinInSamples) % biggerBufferSize;
    
    if ( biggerBufferSize > readPosition + bufferSize)
    {
        buffer.addFromWithRamp(channel, 0, biggerBufferData + readPosition, bufferSize, 1.0, 1.0);
    }
    else
    {
        const int sampleRem = biggerBufferSize - readPosition;
        buffer.addFromWithRamp(channel, 0, biggerBufferData + readPosition, sampleRem,1.0, 1.0);
        buffer.addFromWithRamp(channel, sampleRem, biggerBufferData, bufferSize - sampleRem,1.0, 1.0);
    }
}


/*
 
 auto level = 0.125f;
   
   for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
       buffer.clear (i, 0, buffer.getNumSamples());

   auto* leftBuffer  = buffer.getWritePointer (0);
   auto* rightBuffer = buffer.getWritePointer (1);
   
          for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
          {
              auto currentSample = (float) std::sin (currentAngle);
              currentAngle += angleDelta;
              leftBuffer[sample]  = currentSample * level;
              rightBuffer[sample] = currentSample * level;
          }
 
 */
