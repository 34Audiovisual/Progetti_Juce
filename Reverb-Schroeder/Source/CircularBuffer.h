/*
  ==============================================================================

    CircularBuffer.h
    Created: 6 Dec 2020 2:49:19pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

//template<typename T>
struct CircularBuffer
{
    //using DataType = std::vector<T>;
    
    CircularBuffer(int numElements, float initialValue)
    {
        resize(numElements, initialValue);
    }

    void resize(int s, float fillValue)
    {
        //data.clear();
        //data.resize(s,fillValue);
        
        biggerBuffer.clear();
        biggerBuffer.setSize(1, s);
    }
    /*
    void clear(T fillValue)
    {
        for ( T& i : data)
        {
            i = fillValue;
        }
    }
     */
    
    void write(float t)
    {
        auto idx = writeIndex;
        biggerBuffer.setSample(0, idx, t);

        ++idx;
        if( idx + 1 > biggerBuffer.getNumSamples() )
            idx = 0;
        writeIndex = idx;
    }

    
    juce::AudioBuffer<float>& getBiggerBuffer()
    {
        return biggerBuffer;
    }
    
    int getReadIndex() const
    {
        
        int readIndex = writeIndex + 1;
        if ( readIndex > biggerBuffer.getNumSamples() )
            readIndex = 0;
        
        return readIndex;
    }
    
    size_t getSize() const
    {
        return biggerBuffer.getNumSamples();
    }
    
private:
    //DataType data;
    juce::AudioBuffer<float> biggerBuffer;
    int writeIndex = 0;
};

