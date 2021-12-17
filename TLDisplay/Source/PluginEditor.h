/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ControlerMidiAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ControlerMidiAudioProcessorEditor (ControlerMidiAudioProcessor&);
    ~ControlerMidiAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void startClicked ( int display, int position, juce::String riga1T, juce::String riga2T);
    juce::String makeString (juce::String string);
private:
    juce::Label displayLab, positionLab, error, sent;
    juce::ComboBox displayBox, positionBox;
    juce::TextEditor riga1, riga2;
    juce::TextButton send;
    std::array<int, 8> noteArray1, noteArray2;
    
    juce::StringArray displayArray {"Display 1", "Display 2", "Display 3", "Display 4", "Display 5", "Display 6", "Display 7", "Display 8", "Display 9", "Display 10", "Display 11", "Display 12", "Display 13", "Display 14", "Display 15", "Display 16" };
    
    juce::StringArray positionArray {"Position 1", "Position 2", "Position 3", "Position 4", "Position 5", "Position 6", "Position 7", "Position 8" };
    
    juce::ToggleButton sentButton;
    
    ControlerMidiAudioProcessor& audioProcessor;
    
    juce::ImageComponent logo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlerMidiAudioProcessorEditor)
};
