/*
  ==============================================================================

    CircularBufferNew.h
    Created: 21 Sep 2021 3:39:06pm
    Author:  Diego Capoccitti

  ==============================================================================
*/
#include <JuceHeader.h>
#pragma once
/*
 Inizializzare con la funzione prepare() in prepareToPlay().
 
 Usare nel Processblock, con la funzione fillbuffer(), seguito dalla funzione nextCycle() per far avanzare il writePointer.
 
 nextCycle() deve stare fuori del conteggio dei canali, ed è separato in modo da permettere l'utilizzo del CircularBuffer per diversi scopi, come l'esportazione dei dati del buffer o il delay.
 
 */
struct CircularBuffer
{

    void fillbuffer(int channel, int bufferSize, float* c, juce::AudioBuffer<float>& buffer);
    void prepare(int numChannels, int numSamples);
    float getMagnitude();
    juce::AudioBuffer<float>& getBiggerBuffer() { return biggerBuffer;};
    
    void getFromBiggerBuffer(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& biggerBuffer, int writePosition, int channel);
    
    int getWritePosition () { return writeposition;};
    void nextCycle(int bf)
    {
        writeposition += bf;
        writeposition %=biggerBufferSize;
    }
private:
    int writeposition = 0;
    int biggerBufferSize = 512;
    juce::AudioBuffer<float> biggerBuffer;
};
