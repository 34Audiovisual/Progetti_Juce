#include <JuceHeader.h>
#pragma once

class CircularBuffer
{
public:

    void fillbuffer(int channel, int bufferSize, float* channelData, juce::AudioBuffer<float>& buffer) // ?
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

    juce::AudioBuffer<float>& getBiggerBuffer() { return biggerBuffer; }; // serve a prendere una referenza del buffer grande per poter utilizzare gli stessi dati
                                                                    // del buffer sia nell'istanza CircularBuffer che nell'istanza Delay

    void getFromBiggerBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& biggerBuffer, int writePosition, int channel); // serve a leggere i dati dal buffer più grande
                                                                                                                            // per poterli utilizzare per il delay

    int getWritePosition() { return writeposition; };

    void nextCycle(int bf)
    {
        writeposition += bf;               // fa avanzare il writeposition di una grandezza pari alla grandezza del buffer piccolo ogni volta che lo copiamo nel buffer più grande
        writeposition %= biggerBufferSize; // nel momento in cui il writeposition diventa più grande della grandezza del buffer grande, ricomincia da 0 mantenendo lo scarto e
                                           // e quindi si posiziona nella casella successiva agli ultimi dati scritti del pezzetto avanzato del buffer piccolo
    }

private:
    int writeposition = 0;
    int biggerBufferSize = 512;

    juce::AudioBuffer<float> biggerBuffer;
};
