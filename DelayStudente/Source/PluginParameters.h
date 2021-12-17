#pragma once
#include <JuceHeader.h>

//DelayS
#define MAX_DELAY_TIME          5000.0f

#define NAME_DELAY_TIME         "time"
#define DEFAULT_DELAY_TIME      500.0f

//Feedback
#define NAME_FB				    "fb"
#define DEFAULT_FB			    0.0f

#define FB_SMOOTHING_TIME       0.01f
#define TIME_SMOOTHING_TIME     0.4f

//DelayL
#define NAME_DELAY_TIME_L       "time_l"
#define DEFAULT_DELAY_TIME_L    0.5f

//DelayR
#define NAME_DELAY_TIME_R       "time_r"
#define DEFAULT_DELAY_TIME_R    0.5f

//DryWet
#define DEFAULT_DW              0.5f
#define NAME_DW                 "drywet"

//ChannelConfiguration
#define NAME_CC	                "Configuration"
#define DEFAULT_CC	            true
bool CC = DEFAULT_CC;

AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back(std::make_unique<AudioParameterFloat>(NAME_DW, "Dry/Wet", 0.0f, 1.0f, DEFAULT_DW));
    params.push_back(std::make_unique<AudioParameterFloat>(NAME_DELAY_TIME, "Delay time", NormalisableRange<float>(50.0f, MAX_DELAY_TIME, 1.f, 0.5f), DEFAULT_DELAY_TIME));
    params.push_back(std::make_unique<AudioParameterFloat>(NAME_DELAY_TIME_L, "Delay time L", NormalisableRange<float>(0.0f, MAX_DELAY_TIME, 0.001f, 0.5f), DEFAULT_DELAY_TIME_L));
    params.push_back(std::make_unique<AudioParameterFloat>(NAME_DELAY_TIME_R, "Delay time R", NormalisableRange<float>(0.0f, MAX_DELAY_TIME, 0.001f, 0.5f), DEFAULT_DELAY_TIME_R));
    params.push_back(std::make_unique<AudioParameterFloat>(NAME_FB, "Feedback", NormalisableRange<float>(0.0f, 1.0f, 0.001f, 2.0f), DEFAULT_FB));
    params.push_back(std::make_unique<AudioParameterBool>(NAME_CC, "Channel Configuration", DEFAULT_CC));

    return { params.begin(), params.end() };
}
