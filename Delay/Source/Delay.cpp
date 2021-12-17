/*
  ==============================================================================

    Delay.cpp
    Created: 27 Sep 2021 2:19:59am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#include "Delay.h"

void Delay::getFromBiggerBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& biggerBuffer, int writePosition, int channel)
{
   
    int biggerBufferSize = biggerBuffer.getNumSamples();
    int bufferSize = buffer.getNumSamples();
    int delayTimeInSamples = (int) (sampleRate * delayTimeInMs / 1000);
    
    auto* biggerBufferData = biggerBuffer.getWritePointer (channel);
    const int readPosition = static_cast<int> (biggerBufferSize + writePosition - delayTimeInSamples) % biggerBufferSize;
    
    if ( biggerBufferSize > readPosition + bufferSize)
    {
        buffer.addFromWithRamp(channel, 0, biggerBufferData + readPosition, bufferSize, gainDelay, gainDelay);
    }
    else
    {
        const int sampleRem = biggerBufferSize - readPosition;
        buffer.addFromWithRamp(channel, 0, biggerBufferData + readPosition, sampleRem, gainDelay, gainDelay);
        buffer.addFromWithRamp(channel, sampleRem, biggerBufferData, bufferSize - sampleRem, gainDelay, gainDelay);
    }
}
