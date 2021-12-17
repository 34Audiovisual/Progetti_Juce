/*
  ==============================================================================

    CircularBufferNew.h
    Created: 21 Sep 2021 3:39:06pm
    Author:  Diego Capoccitti

  ==============================================================================
*/
#include <JuceHeader.h>
#pragma once

struct CircularBuffer
{

    void fillbuffer(int channel, int bufferSize, float* c);
    void prepare(int numChannels, int numSamples);
    float getMagnitude();
private:
    int writeposition = 0;
    int biggerBufferSize = 512;
    juce::AudioBuffer<float> biggerBuffer;
};
