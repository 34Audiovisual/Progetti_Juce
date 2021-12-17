#include <JuceHeader.h>
#pragma once

struct CircularBuffer
{

    void fillbuffer(int channel, int bufferSize, float* channelData, AudioBuffer<float>& buffer)
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

    void prepare(int numChannels, int numSamples)
    {

        biggerBuffer.setSize(numChannels, numSamples);
        biggerBufferSize = numSamples;
        biggerBuffer.clear();
    }

    float getMagnitude()
    {
        return biggerBuffer.getMagnitude(0, biggerBufferSize);
    }

    AudioBuffer<float>& getBiggerBuffer() { return biggerBuffer; };

    void getFromBiggerBuffer(AudioBuffer<float>& buffer, AudioBuffer<float>& biggerBuffer, int writePosition, int channel);

    int getWritePosition() { return writeposition; };

    void nextCycle(int bf)
    {
        writeposition += bf;
        writeposition %= biggerBufferSize;
    }

private:
    int writeposition = 0;
    int biggerBufferSize = 512;
    
    AudioBuffer<float> biggerBuffer;
};
