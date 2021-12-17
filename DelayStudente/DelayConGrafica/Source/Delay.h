#pragma once
#include <JuceHeader.h>

#include "PluginParameters.h"

class Delay
{
public:

    //per inizializzare l'istanza di delay
    void prepare(double samplerate)
    {
        sr = samplerate;
    }

    void getFromBiggerBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& biggerBuffer, int writePosition, int channel)
    {
        int biggerBufferSize = biggerBuffer.getNumSamples();
        int bufferSize = buffer.getNumSamples();
        int delayTimeInSamples = (int)(sr * delayTime / 1000);

        auto* biggerBufferData = biggerBuffer.getWritePointer(channel);
        const int readPosition = static_cast<int> (biggerBufferSize + writePosition - delayTimeInSamples) % biggerBufferSize; 
        // const int readPosition = (biggerBufferSize + writePosition - delayTimeInSamples) % biggerBufferSize;

        if (biggerBufferSize > readPosition + bufferSize)
        {
            buffer.addFromWithRamp(channel, 0, biggerBufferData + readPosition, bufferSize, feedback, feedback);
        }
        else
        {
            const int sampleRem = biggerBufferSize - readPosition;
            buffer.addFromWithRamp(channel, 0, biggerBufferData + readPosition, sampleRem, feedback, feedback);
            buffer.addFromWithRamp(channel, sampleRem, biggerBufferData, bufferSize - sampleRem, feedback, feedback);
        }
    }

    // dobbiamo settare il delay in maniera che valga sia per la versione stereo che per la versione dual mono
    void updateDelay(float d, float f)
    {
        delayTime = d;
        feedback = f;
    }


private:

    float delayTime = 0.0f;
    double sr = 44100.0;
    float feedback = 0.0f;
};
