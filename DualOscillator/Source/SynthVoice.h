/*
  ==============================================================================

    SynthVoice.h
    Created: 26 Oct 2021 12:31:18am
    Author:  Diego Capoccitti

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/AdsrData.h"
#include "Tube.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    void update ( const float attack, const float decay, const float sustain, const float release);
    void setGain (float g) { gain.setGainLinear(g);};
    void setOn () { adsr.noteOn();};
    void setOff () { adsr.noteOff();};
    void setFreq (float f) { osc.setFrequency(f);};
    void setFreqFine (float ff)
    { 
        auto currentFreq = osc.getFrequency();
        auto newFreq = currentFreq + ff;
        osc.setFrequency(newFreq);
    };
    float getFreq () { return osc.getFrequency();};
    
    void setTube (int choice)
    {
        float ln1_coeff = 1.f;
        float inGainTube = -2.f;
        float outGainTube = 0.1f;
        float driveTube = 0.1f; //tra -1.0f e 10.f
        float blendTube = 10.f; //blend (-10.f == 'tape' -> +10.f == 'tube')
        
        switch (choice)
        {
            case 0:
                //DBG("hai scelto 0.01V");
                ln1_coeff = 0.01f;
                driveTube = 0.1f;
                break;
                
            case 1:
                //DBG("hai scelto 0.1V");
                ln1_coeff = 0.1f;
                driveTube = 0.1f;
                break;
                
            case 2:
                //DBG("hai scelto 1V");
                ln1_coeff = 1.f;
                driveTube = 0.1f;
                break;
                
            case 3:
                //DBG("hai scelto 10V");
                ln1_coeff = 1.f;
                driveTube = 9.f;
                break;
                
            default:
              
                break;
        }
        
        tube.feedCoontrols(ln1_coeff, inGainTube, driveTube, blendTube, outGainTube);
    }
    
    void setWaveform (int choice)
    {
        switch (choice)
        {
            case 0:
                osc.initialise([](float x) { return std::sin (x);});
                break;
            case 1:
                osc.initialise([](float x) { return x / juce::MathConstants<float>::pi;});
                break;
                
            default:
                osc.initialise([](float x) { return std::sin (x);});
                break;
        }

        //std::function<float(float x)> prova = [](float x) { return std::sin (x);};
    }

private:
    AdsrData adsr;
    juce::AudioBuffer<float> synthBuffer;
    
    
    juce::dsp::Oscillator<float> osc { [](float x) { return std::sin (x); }};
    //juce::dsp::Oscillator<float> osc { [](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 200 };
    juce::dsp::Gain<float> gain;

    // return std::sin (x); //Sine Wave
    // return x / MathConstants<float>::pi; // Saw Wave
    // return x < 0.0f ? -1.0f : 1.0f;  // Square Wave
    
    bool isPrepared = false;
    
    Tube tube;
};
