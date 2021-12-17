/*
  ==============================================================================

    Granulator.cpp
    Created: 14 Dec 2021 1:48:15am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#include "Granulator.h"

Granulator::Granulator ( const juce::AudioBuffer<float>& fileToUse)
    : loadedFile (fileToUse)
{
    jassert (loadedFile.getNumChannels() == 1);
    sizeFile = loadedFile.getNumSamples();
}

void Granulator::setPhase (float phase)
{
    makeGrain();
    currentIndex = tableSize.get() * phase;
}

float Granulator::getNextSample() noexcept
{
    if (isFileLoaded())
    {
        auto currentSample = interpolation(currentIndex, tableSize.get(), posCycle);

        currentIndex += grainPitch;
        //Calcola il sample della finestra di inviluppo (trapezio)
        auto sampleWin = win.applyTrapWindow(0.20, tableSize.get(), currentIndex);
        
        if ((currentIndex) > (float) tableSize.get() || currentIndex < 0.f)
        {
            if (currentIndex < 0)
            {
                endCycle = true;
                makeGrain();
                currentIndex += (float)tableSize.get();
            }
            else
            {
                currentIndex -= (float) tableSize.get();
                endCycle = true;
                makeGrain();
            }
            
           // currentIndex < 0 ? currentIndex += (float)tableSize.get()  : currentIndex -= (float) tableSize.get();
            
            if (randomize)
            {
                juce::Random r;
                float rand1 = r.nextFloat();
                phase = r.nextFloat();
                pos.set( sizeFile * rand1);
            }
        }
        else
            endCycle = false;

        return ( currentSample) * sampleWin;
    }
    else
        return 0.f;
}

void Granulator::makeGrain()
{
    requestChange = true;
    sizeFile = loadedFile.getNumSamples();
    juce::Random r;
    //aggiungere qui la variazione casuale di posizione
    position = pos.get();
    auto posRnd = ((deltaPos * r.nextFloat())/2.f) * sizeFile;
    //aggiungere qui la variazione casuale di pitch
    grainPitch = pitch.get();
    auto pitchRnd = deltaPitch *( r.nextFloat() - 0.5);
    //aggiungere qui la variazione casuale di grainLen
    grainLenght = len.get();
    auto lenRnd = deltaLen * (r.nextFloat()) * len.get() * 0.5f;
    
    if (requestChange && endCycle)
    {
        position += posRnd;
        grainLenght += lenRnd;
        grainPitch += pitchRnd;
        auto nSamples = grainLenght * sampleR / 1000;

        if  (nSamples + position > sizeFile)
        {
            position = sizeFile - nSamples;
            
            if (position < 0)
            {
                position = 0;
                nSamples = sizeFile;
            }
            
            pos.set(position);
        }
        
        tableSize.set((int)nSamples - 1);
        posCycle = position;
        requestChange = false;
    }
}

float Granulator::interpolation (float index, int tSize, int pos)
{
    auto index0 = (unsigned int) index;
    auto index1 = index0 == (tSize - 1) ? (unsigned int) 0 : index0 + 1;

    auto frac = index - (float) index0;

    auto* table = loadedFile.getReadPointer(0, pos);
    auto value0 = table[index0];
    auto value1 = table[index1];
    
    auto sample = value0 + frac * (value1 - value0);
    
    return sample;
}

void Granulator::setPosition( float posNorm)
{
    //DBG(posNorm);
    pos.set( static_cast<int>(posNorm * loadedFile.getNumSamples()));
    makeGrain();
}
