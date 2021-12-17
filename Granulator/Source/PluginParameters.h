#pragma once
#include <JuceHeader.h>

// PositionL

#define NAME_POSITIONL     "positionL"
#define DEFAULT_POSITIONL  0.0f

// PositionR
#define NAME_POSITIONR     "positionR"
#define DEFAULT_POSITIONR  0.0f

//spread
#define MAX_SPREAD   100.0f
#define MIN_SPREAD   0.0f

#define NAME_SPREAD     "spread"
#define DEFAULT_SPREAD  50.f

//grainLenght
#define MAX_GRAIN_LENGHT   2000.0f
#define MIN_GRAIN_LENGHT   10.0f

#define NAME_GRAIN_LENGHT     "grainLen"
#define DEFAULT_GRAIN_LENGHT  80.0f

//grainPitch
#define MAX_GRAIN_PITCH   200.0f
#define MIN_GRAIN_PITCH   -200.f

#define NAME_GRAIN_PITCH     "grainPitch"
#define DEFAULT_GRAIN_PITCH  1.0f

//onOff
#define NAME_ONOFF     "onOff"
#define DEFAULT_ONOFF true

//random
#define NAME_RANDOM     "random"
#define DEFAULT_RANDOM false

//link
#define NAME_LINK     "link"
#define DEFAULT_LINK false

//mono
#define NAME_MONO     "mono"
#define DEFAULT_MONO false

//voices
#define NAME_NUM_VOICES     "voices"
#define DEFAULT_NUM_VOICES 1
#define MAX_NUM_VOICES   10
#define MIN_NUM_VOICES   1.f

//deltaPos
#define NAME_DELTAPOS     "deltaPos"
#define DEFAULT_DELTAPOS 0.f
#define MAX_DELTAPOS   100.f
#define MIN_DELTAPOS   0.f

//deltaLen
#define NAME_DELTALEN    "deltaLen"
#define DEFAULT_DELTALEN 0.f
#define MAX_DELTALEN   100.f
#define MIN_DELTALEN   0.f

//deltaLen
#define NAME_DELTAPITCH   "deltaPitch"
#define DEFAULT_DELTAPITCH 0.f
#define MAX_DELTAPITCH   200.f
#define MIN_DELTAPITCH   0.f

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_POSITIONL, "PositionL", 0.0f, 100.0f, DEFAULT_POSITIONL));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_POSITIONR, "PositionR", 0.0f, 100.0f, DEFAULT_POSITIONR));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_SPREAD, "Spread", MIN_SPREAD, MAX_SPREAD, DEFAULT_SPREAD));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_GRAIN_LENGHT, "GrainLenght", MIN_GRAIN_LENGHT, MAX_GRAIN_LENGHT, DEFAULT_GRAIN_LENGHT));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_GRAIN_PITCH, "GrainPitch", MIN_GRAIN_PITCH, MAX_GRAIN_PITCH, DEFAULT_GRAIN_PITCH));

    params.push_back(std::make_unique<juce::AudioParameterBool>(NAME_ONOFF, "OnOff", DEFAULT_ONOFF));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(NAME_RANDOM, "Random", DEFAULT_RANDOM));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(NAME_LINK, "Link", DEFAULT_LINK));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(NAME_MONO, "Mono", DEFAULT_MONO));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(NAME_NUM_VOICES, "Voices", MIN_NUM_VOICES, MAX_NUM_VOICES, DEFAULT_NUM_VOICES));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_DELTAPOS, "GrainPos", MIN_DELTAPOS, MAX_DELTAPOS, DEFAULT_DELTAPOS));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_DELTALEN, "GrainLen", MIN_DELTALEN, MAX_DELTALEN, DEFAULT_DELTALEN));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_DELTAPITCH, "GrainPitch", MIN_DELTAPITCH, MAX_DELTAPITCH, DEFAULT_DELTAPITCH));
    
    //params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_DELTAPITCH, "GrainPitch", MIN_DELTAPITCH, MAX_DELTAPITCH, DEFAULT_DELTAPITCH));
    
	return {params.begin(), params.end()};
}
