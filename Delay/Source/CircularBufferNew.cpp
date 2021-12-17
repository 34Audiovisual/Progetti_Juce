/*
  ==============================================================================

    CircularBufferNew.cpp
    Created: 21 Sep 2021 3:39:06pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#include "CircularBufferNew.h"
void CircularBuffer::fillbuffer(int channel, int bufferSize, float* channelData, juce::AudioBuffer<float>& buffer)
{
    if (biggerBufferSize > bufferSize + writeposition)
    {
        biggerBuffer.copyFrom(channel, writeposition, channelData, bufferSize);
    }
    else
    {
        auto numSamplesToEnd = biggerBufferSize - writeposition;
        biggerBuffer.copyFrom(channel, writeposition, channelData, numSamplesToEnd);
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;
        biggerBuffer.copyFrom(channel, 0, channelData, numSamplesAtStart);
    }
}

void CircularBuffer::prepare(int numChannels, int numSamples)
{
    
    biggerBuffer.setSize(numChannels, numSamples);
    biggerBufferSize = numSamples;
    biggerBuffer.clear();
}

float CircularBuffer::getMagnitude()
{
    return biggerBuffer.getMagnitude(0, biggerBufferSize);
}


