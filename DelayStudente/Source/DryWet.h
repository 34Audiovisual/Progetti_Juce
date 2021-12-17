#pragma once
#include <JuceHeader.h>

#include "PluginParameters.h"

class DryWetter : public AudioProcessorValueTreeState::Listener
{
public:
	DryWetter()
	{
		updateInternalState(DEFAULT_DW);
	}

	~DryWetter() {};

	void prepareToPlay(const int maxSamplesPerBlock)
	{
		drySignal.setSize(2, maxSamplesPerBlock);
	}

	void releaseResources()
	{
		drySignal.setSize(0, 0);
	}

	void storeFrom(const AudioBuffer<float>& inputSignal)
	{
		const auto numSamples = inputSignal.getNumSamples();

		for (int ch = inputSignal.getNumChannels(); --ch >= 0;)
			drySignal.copyFrom(ch, 0, inputSignal, ch, 0, numSamples);

		// se avessi mono-in stereo-out potrei tenere drySignal sempre stereo e fare
		// sourceCh = jmin(ch, numInputChannels-1);
		// drySignal.copyFrom(ch, 0, inputSignal, sourceCh, 0, numSamples);
	}

	void mergeTo(AudioBuffer<float>& outputSignal)
	{
		const auto numSamples = outputSignal.getNumSamples();

		if (doSmooth)
		{
			outputSignal.applyGainRamp(0, numSamples, wetGainOld, wetGain);

			for (int ch = outputSignal.getNumChannels(); --ch >= 0;)
				outputSignal.addFromWithRamp(ch, 0, drySignal.getReadPointer(ch), numSamples, dryGainOld, dryGain);

			doSmooth = false;
		}
		else
		{
			outputSignal.applyGain(wetGain);

			for (int ch = outputSignal.getNumChannels(); --ch >= 0;)
				outputSignal.addFrom(ch, 0, drySignal, ch, 0, numSamples, dryGain);
		}
	}

private:
	void parameterChanged(const String& paramID, float newValue) override
	{
		if (paramID == NAME_DW)
			updateInternalState(newValue);
	}

	void updateInternalState(float newValue)
	{
		dryGainOld = dryGain;
		wetGainOld = wetGain;

		dryGain = sqrt(1.0f - newValue);
		wetGain = sqrt(newValue);

		doSmooth = true;
	}

	AudioBuffer<float> drySignal;

	float dryGain, dryGainOld;
	float wetGain, wetGainOld;

	bool doSmooth;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DryWetter)
};