/*
  ==============================================================================

    CircularBufferNew.cpp
    Created: 21 Sep 2021 3:39:06pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#include "CircularBufferNew.h"
void CircularBuffer::fillbuffer(int channel, int bufferSize, float* channelData)
{
    if (biggerBufferSize > bufferSize + writeposition)
    {
        biggerBuffer.copyFromWithRamp(channel, writeposition, channelData, bufferSize, 0.1f, 0.1f);
    }
    else
    {
        auto numSamplesToEnd = biggerBufferSize - writeposition;
        biggerBuffer.copyFromWithRamp(channel, writeposition, channelData, numSamplesToEnd, 0.1f, 0.1f);
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;
        biggerBuffer.copyFromWithRamp(channel, 0, channelData, numSamplesAtStart, 0.1f, 0.1f);
    }
    
    writeposition += bufferSize;
    writeposition %=biggerBufferSize;
}

void CircularBuffer::prepare(int numChannels, int numSamples)
{
    biggerBuffer.setSize(numChannels, numSamples);
    biggerBufferSize = numSamples;
}

float CircularBuffer::getMagnitude()
{
    return biggerBuffer.getMagnitude(0, biggerBufferSize);
}
