/*
  ==============================================================================

    Created: 30 Nov 2021 9:47:47pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
//enum WindowType
//{
//    Hann,
//    Gauss,
//    QuasiGauss,
//    Triangolare,
//    Trapezio,
//    Expodec,
//    Rexpodec,
//    Impulso,
//    Blackman,
//    Welch
//};

class Window
{
public:
    Window() {}
    ~Window(){}

    float applyTrapWindow(float slope, int tSize, float cIndex)
    {
        auto samplesInSlope = (int)(tSize * slope);
        float sampleWin;
        
        if (cIndex < samplesInSlope)
        {
            sampleWin = (1.f/(float)(samplesInSlope)) * cIndex;
        }
        
        else if (cIndex > (tSize - samplesInSlope))
        {
            sampleWin = (float)(tSize - cIndex) / (float)samplesInSlope;
        }
        else
        {
            sampleWin = 1.f;
        }
        
        return sampleWin;
    }
private:

};
//=============================================

class Granulator
{
public:
    Granulator ( const juce::AudioBuffer<float>& fileToUse);
    
    void prepare ( float sampleRate) { sampleR = sampleRate; }
    void setPhase (float phase);
    
    float getNextSample() noexcept;
    void makeGrain();
    float interpolation (float index, int tSize, int pos);
    void setPosition( float posNorm);
    
    void setGrainLenght ( float ms) { len.set(ms);makeGrain();}
    void setGrainPitch ( float ph) { pitch.set(ph);makeGrain();}
    void setDeltaPos ( float dp) { deltaPos = dp; makeGrain();}
    void setDeltaLen ( float dl) { deltaLen = dl;}
    void setDeltaPitch ( float dpt) { deltaPitch = dpt;}
    void setFileLoaded ( bool sf) {fileLoaded = sf; }
    void setON ( bool so) {isOn = so; }
    void setRandom ( bool ran) {randomize = ran; }
    void setEndCycle( bool cycle) {endCycle = cycle;}
    
    float getIndex() {return currentIndex;}
    float getPosition() {return ((float)pos.get() / (float)sizeFile);} // position normalizzata 0-1
    float getGrainLen() {return len.get();}
    float getGrainPitch() {return pitch.get();}
    bool  isFileLoaded() {return fileLoaded;}
    bool isActive() {return isOn;}
    
private:
    float currentIndex = 0.0f;
    const juce::AudioBuffer<float>& loadedFile;
    int sizeFile;
    juce::Atomic<int> tableSize;
    float sampleR = 44100;
    Window win;
    
    float grainLenght = 80.f;
    int position;
    int posCycle = 0;
    float grainPitch = 1;
    float phase = 0.5;
    juce::Atomic<int> pos;
    juce::Atomic<float> len;
    juce::Atomic<float> pitch;
    float deltaLen = 0.f, deltaPitch = 0.f, deltaPos = 0.f;
    
    bool fileLoaded { false };
    bool requestChange { false };
    bool endCycle { true };
    bool randomize { false };
    bool isOn { true};
};

//==============================================================================

class GranulatorVoices
{
public:
    GranulatorVoices(juce::AudioBuffer<float>& lf, std::size_t& numVoices) : loadedFile(lf), numGran(numVoices)
    {
        prepareGranulatorsVoices();
    }
    ~GranulatorVoices(){}
    
    void prepareGranulatorsVoices ()
    {
        granulators.clear();

        for (int i = 0 ; i < numGran; ++i)
        {
            Granulator granSing {loadedFile};
            granulators.push_back(granSing);
        }
    }
    
    void setParameters (double sampleRate, float grainLenght, float position, float  gPitch, bool ran, bool on, float dPos, float dLen, float dPitch)
    {
        phase1 = 0.f;
        
        for (int i = 0 ; i < numGran ; ++i)
        {
            phase1 = (float)i / (float)numGran;
            granulators[i].setEndCycle(true);
            granulators[i].prepare(sampleRate);
            granulators[i].setPosition(position);
            granulators[i].setGrainLenght(grainLenght);
            granulators[i].setGrainPitch(gPitch);
            
            if ( i == 0)
            {
                granulators[i].setDeltaPos(0);
                granulators[i].setDeltaLen(0);
                granulators[i].setDeltaPitch(0);
            }
            else
            {
                granulators[i].setDeltaPos(dPos);
                granulators[i].setDeltaLen(dLen);
                granulators[i].setDeltaPitch(dPitch);
            }

            //granulators[i].setPhase(phase1);
            granulators[i].setRandom(ran);
            granulators[i].setON(on);
            granulators[i].setFileLoaded(true);
            granulators[i].makeGrain();
        }
    }
    
    void setGrainLenght ( float grainLen)
    {
        for (int i = 0 ; i < numGran ; ++i)
        {
            granulators[i].setGrainLenght(grainLen);
        }
    }
    
    void setPosition(float pos)
    {
        for (int i = 0 ; i < numGran ; ++i)
        {
            granulators[i].setPosition(pos);
        }
    }
    
    void setPitch ( float pitch)
    {
        for (int i = 0 ; i < numGran ; ++i)
        {
            granulators[i].setGrainPitch(pitch);
        }
    }
    
    void setOn(bool on)
    {
        for (int i = 0 ; i < numGran ; ++i)
        {
            granulators[i].setON(on);
        }
    }
    
    void setRandom(bool rnd)
    {
        for (int i = 0 ; i < numGran ; ++i)
        {
            granulators[i].setRandom(rnd);
        }
    }
    
    void setDeltaLen (float deltaL)
    {
        granulators[0].setDeltaLen(0);
        
        for (int i = 1 ; i < numGran ; ++i)
        {
            granulators[i].setDeltaLen(deltaL);
        }
    }
    
    void setDeltaPos (float deltaP)
    {
        granulators[0].setDeltaPos(0);
        
        for (int i = 1 ; i < numGran ; ++i)
        {
            granulators[i].setDeltaPos(deltaP);
        }
    }
    
    void setDeltaPitch (float deltaPi)
    {
        granulators[0].setDeltaPitch(0);
        
        for (int i = 1 ; i < numGran ; ++i)
        {
            granulators[i].setDeltaPitch(deltaPi);
        }
    }
    
    void setFileLoaded (bool fl)
    {
        for (int i = 0 ; i < numGran ; ++i)
        {
            granulators[i].setFileLoaded(fl);
        }
    }
    
    float nextFloat()
    {
        float sample = 0;
        std::atomic<float> s (0.f);
        
        for (int i = 0 ; i < numGran ; ++i)
        {
            sample += granulators[i].getNextSample() * 0.5;
            s.store(sample);
        }
        
        return s.load();
    }
    
    Granulator& gran0 () {return granulators[0];}
    void clear() { granulators.clear();}
    
private:
    juce::AudioBuffer<float>& loadedFile;
    //std::vector<std::atomic<Granulator>>  granulators;
    //std::vector<std::thread> threads;
    std::vector<Granulator>  granulators;
    std::size_t& numGran;
    float phase1 = 0;
    juce::Random r;
    float deltaLen = 0.f, deltaPitch = 0.f, deltaPos = 0.f;
};
