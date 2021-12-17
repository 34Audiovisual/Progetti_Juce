/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SamplerAudioProcessor  : public juce::AudioProcessor

{
public:
    //==============================================================================
    SamplerAudioProcessor();
    ~SamplerAudioProcessor() override;
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadFile()
    {
        mSampler.clearSounds();
        
        chooser = std::make_unique<juce::FileChooser> ("Select a file to play...");
        
        auto chooserFlags = juce::FileBrowserComponent::openMode
                          | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
        {
            auto file = fc.getResult();

            if (file != juce::File{})
            {
                auto* reader = mFormatManager.createReaderFor (file);

                if (reader != nullptr)
                {
                    juce::BigInteger range;
                    range.setRange(0, 128, true);
                    mSampler.addSound(new juce::SamplerSound("Sample", *reader, range, 60, 0.1, 0.1, 10));
                }
                
                reader->~AudioFormatReader();
            }
        });
    }
    
    void loadFile ( const juce::String& path)
    {
        mSampler.clearSounds();
        
        auto file = juce::File (path);
        //mFormatManager = std::make_unique<juce::AudioFormatManager> ();
        auto* reader = mFormatManager.createReaderFor (file);
        
        auto sampleLenght = static_cast<int>(reader->lengthInSamples);
        
        mWaveForm.setSize(1, sampleLenght);
        reader->read(&mWaveForm, 0, sampleLenght, 0, true, false);
        
//        auto buffer = mWaveForm.getReadPointer(0);
//        for (int  sample = 0; sample < mWaveForm.getNumSamples(); ++sample)
//        {
//            DBG (buffer[sample]);
//        }

        if (reader != nullptr)
        {
            juce::BigInteger range;
            range.setRange(0, 128, true);
            mSampler.addSound(new juce::SamplerSound("Sample", *reader, range, 60, 0.1, 0.1, 10));
        }
        
        reader->~AudioFormatReader();
    }
    
    int getNumSound () { return mSampler.getNumSounds(); }
    juce::AudioBuffer<float>& getWaveForm() { return mWaveForm;}
    
private:
    juce::Synthesiser mSampler;
    const int  nNumVoices {3};
    
    juce::AudioFormatManager mFormatManager;
                                     
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioBuffer<float> mWaveForm;

    //std::unique_ptr<juce::AudioFormatReader> mFormatReader;
   // juce::AudioTransportSource transportSource;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessor)
};
