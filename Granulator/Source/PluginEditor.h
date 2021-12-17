/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
 

  - OK sistemare pitch var reverse
  - OK ?? sistemare lo stereo/mono per protools
  - OK versione stereo
  - sistemare inizio fermo (si attiva se cambio le voci)
 - Grafica?
 - OK spread
 
 
 
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DragWaveform.h"

//==============================================================================
/**
*/
class GranulatorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GranulatorAudioProcessorEditor (GranulatorAudioProcessor&);
    ~GranulatorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider positionSliderL;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> positionAttachmentL;
    juce::Label posLabelL;
    
    juce::Slider positionSliderR;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> positionAttachmentR;
    juce::Label posLabelR;
    
    juce::Slider spreadSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> spreadAttachment;
    juce::Label spreadLabel;
    
    juce::Slider grainLenghtSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> grainLenghtAttachment;
    juce::Label lenLabel;
    
    juce::Slider grainPitchSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> grainPitchAttachment;
    juce::Label pitchLabel;
    
    juce::ToggleButton onOffButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> onOffAttachment;
    
    juce::ToggleButton randomButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> randomAttachment;
    
    juce::ToggleButton monoButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> monoAttachment;
    
    juce::ToggleButton linkButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> linkAttachment;
    
    juce::Slider voicesSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> voicesAttachment;
    juce::Label voicesLabel;
    
    juce::Slider deltaPosSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> deltaPosAttachment;
    juce::Label deltaPosLabel;
    
    juce::Slider deltaLenSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> deltaLenAttachment;
    juce::Label deltaLenLabel;
    
    juce::Slider deltaPitchSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> deltaPitchAttachment;
    juce::Label deltaPitchLabel;
    
    juce::TextButton playButton {"Play file"};
    
    GranulatorAudioProcessor& audioProcessor;
    DragWaveform waveform;
    
    juce::ImageComponent logo;
    juce::HyperlinkButton link { "http://www.diegocapoccitti.com", juce::URL("http://www.diegocapoccitti.com") };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranulatorAudioProcessorEditor)
};
