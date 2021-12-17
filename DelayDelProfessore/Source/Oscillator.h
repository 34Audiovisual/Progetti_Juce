#pragma once
#include <JuceHEader.h>
#include "PluginParameters.h"

class NaiveOscillator : public AudioProcessorValueTreeState::Listener
{
public:
	NaiveOscillator() 
	{
		frequency.setCurrentAndTargetValue(DEFAULT_RATE);
	}

	~NaiveOscillator() {};

	void pepareToPlay(double sr)
	{
		sampleRate = sr;
		frequency.reset(sr, RATE_SMTH);
	}

	void getNextAudioBlock(AudioBuffer<float>& buffer, const int numSamples)
	{
		const auto numChannels = buffer.getNumChannels();
		auto** bufferData = buffer.getArrayOfWritePointers();

		for (int smp = 0; smp < numSamples; ++smp)
		{
			const float sampleValue = getNextAudioSample();

			for (int ch = 0; ch < numChannels; ++ch)
				bufferData[ch][smp] = sampleValue;
		}
	}

	float getNextAudioSample()
	{
		float sampleValue;

		switch (waveform)
		{
		case 0: // Sinusoide
			sampleValue = sin(2.0f * float_Pi * normalizedPhaseValue);
			break;
		case 1: // Triangolare
			sampleValue = 4.0f * abs(normalizedPhaseValue - 0.5f) - 1.0f;
			break;
		case 2: // Sawtooth crescente(per abbassare il pitch)
			sampleValue = 2.0f * normalizedPhaseValue - 1.0f;
			break;
		case 3: // Sawtooth decrescante (per alzare il pitch)
			sampleValue = -2.0f * normalizedPhaseValue + 1.0f;
			break;
		default:
			sampleValue = 0.0f;
		}

		const float phaseIncrement = frequency.getNextValue() / sampleRate;

		normalizedPhaseValue += phaseIncrement; // + delta su uno dei canali se stereo
		normalizedPhaseValue -= static_cast<int>(normalizedPhaseValue);

		return sampleValue;
	}

private:
	void parameterChanged(const String& paramID, float newValue) override
	{
		if (paramID == NAME_RATE)
			frequency.setTargetValue(newValue);
		if (paramID == NAME_WF)
			waveform = roundToInt(newValue);
	}

	int waveform = DEFAULT_WF;
	double sampleRate = 1.0;
	float normalizedPhaseValue = 0.0f; // Diventa un array in caso di osc stereo
	SmoothedValue<float, ValueSmoothingTypes::Multiplicative> frequency;
};