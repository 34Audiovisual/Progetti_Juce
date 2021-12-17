#pragma once
#include <JuceHeader.h>

// OscillatoreX
#define NAME_AMP_OSCX                 "ampOscX"
#define DEFAULT_AMP_OSCX              0.9f

#define NAME_PHASE_OSCX				  "phaseOscX"
#define DEFAULT_PHASE_OSCX			  0.0f

#define NAME_FREQ_OSCX                "freqOscX"
#define DEFAULT_FREQ_OSCX             110.0f

#define NAME_MULT_OSCX                "multOscX"
#define DEFAULT_MULT_OSCX             1.0f


// OscillatoreY
#define NAME_AMP_OSCY                 "ampOscY"
#define DEFAULT_AMP_OSCY              0.9f

#define NAME_PHASE_OSCY                "phaseOscY"
#define DEFAULT_PHASE_OSCY            1.0f

#define NAME_FREQ_OSCY                "freqOscY"
#define DEFAULT_FREQ_OSCY             110.1f

#define NAME_MULT_OSCY                "multOscY"
#define DEFAULT_MULT_OSCY             1.0f

#define NAME_DIM                "dimension"
#define DEFAULT_DIM            0.5f

#define NAME_OFFSETX                "offsetX"
#define DEFAULT_OFFSETX            0.0f

#define NAME_OFFSETY                "offsetY"
#define DEFAULT_OFFSETY            0.0f

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_AMP_OSCX, "Amp OscX", 0.0f, 1.0f, DEFAULT_AMP_OSCX));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_PHASE_OSCX, "Phase OscX", 0.0f, 2.0f, DEFAULT_PHASE_OSCX));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_FREQ_OSCX, "Freq OscX", 20.0f, 2000.0f, DEFAULT_FREQ_OSCX));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_MULT_OSCX, "Multip Freq OscX", 1.0f, 20.0f, DEFAULT_MULT_OSCX));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_AMP_OSCY, "Amp OscY", 0.0f, 1.0f, DEFAULT_AMP_OSCY));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_PHASE_OSCY, "Phase OscY", 0.0f, 2.0f, DEFAULT_PHASE_OSCY));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_FREQ_OSCY, "Freq OscY", 20.0f, 2000.0f, DEFAULT_FREQ_OSCY));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_MULT_OSCY, "Multip Freq OscY", 1.0f, 20.0f, DEFAULT_MULT_OSCY));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_DIM, "Dimension", 0.0f, 0.95f, DEFAULT_DIM));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_OFFSETX, "Offset X", -1.0f, 1.00f, DEFAULT_OFFSETX));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(NAME_OFFSETY, "Offset Y", -1.0f, 1.00f, DEFAULT_OFFSETY));

	return {params.begin(), params.end()};
}
