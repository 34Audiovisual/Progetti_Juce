#pragma once
#include <JuceHeader.h>

// Delay
#define MAX_DELAY_TIME   5.0f

#define NAME_DELAY_TIME     "time"
#define DEFAULT_DELAY_TIME  0.5f

#define NAME_FB				"fb"
#define DEFAULT_FB			0.0f

#define FB_SMOOTHING_TIME   0.01f
#define TIME_SMOOTHING_TIME 0.4f

// DryWet
#define DEFAULT_DW  0.5f
#define NAME_DW     "drywet"

// Osc
#define NAME_RATE      "rate"
#define DEFAULT_RATE   1.0f
#define RATE_SMTH      0.05f

#define NAME_WF		   "waveform"
#define DEFAULT_WF     0

// Mod amount
#define DEFAULT_AMT  0.0f
#define NAME_AMT     "amount"

static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
	std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back(std::make_unique<AudioParameterFloat>(NAME_DW, "Dry/Wet", 0.0f, 1.0f, DEFAULT_DW));
    params.push_back(std::make_unique<AudioParameterFloat>(NAME_DELAY_TIME, "Delay time", NormalisableRange<float>(0.0f, MAX_DELAY_TIME, 0.001f, 0.5f), DEFAULT_DELAY_TIME));
    params.push_back(std::make_unique<AudioParameterFloat>(NAME_FB, "Feedback", NormalisableRange<float>(0.0f, 1.0f, 0.001f, 2.0f), DEFAULT_FB));
    params.push_back(std::make_unique<AudioParameterFloat>(NAME_RATE, "LFO freq.", NormalisableRange<float>(0.1f, 20.0f, 0.01f, 0.5f), DEFAULT_RATE));
    params.push_back(std::make_unique<AudioParameterFloat>(NAME_AMT, "Mod. amount", NormalisableRange<float>(0.0f, 1.0f, 0.001f, 0.5f), DEFAULT_AMT));
    params.push_back(std::make_unique<AudioParameterChoice>(NAME_WF, "Waveform", StringArray{ "Sine","Triangular","Saw up","Saw down" }, DEFAULT_WF));

	return {params.begin(), params.end()};
}
