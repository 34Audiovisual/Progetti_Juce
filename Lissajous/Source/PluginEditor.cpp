/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LissajousAudioProcessorEditor::LissajousAudioProcessorEditor (LissajousAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(scope);
    startTimer(30);
    setSize (400, 800);
    
    // osc X
    makeSlider(ampXSlider);
    makeSlider(freqXSlider);
    makeSlider(multXSlider);
    makeSlider(phaseXSlider);
    
    // osc Y
    makeSlider(ampYSlider);
    makeSlider(freqYSlider);
    makeSlider(multYSlider);
    makeSlider(phaseYSlider);
    
    // gen
    makeSlider(dimSlider);
    makeSlider(linkedSlider);
    linkedSlider.setRange(20, 2000, 0.1);
    linkedSlider.onValueChange = [this]
    {
        freqXSlider.setValue(linkedSlider.getValue());
        freqYSlider.setValue(linkedSlider.getValue());
    };
    makeSlider(velSlider);
    velSlider.setRange(-1.0, 1.0, 0.01);
    velSlider.setValue(0.0);
    velSlider.onValueChange = [this]
    {
        freqYSlider.setValue(freqXSlider.getValue() + velSlider.getValue());
    };
    makeSlider(offXSlider);
    offXSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    makeSlider(offYSlider);
    offYSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    
    //attachment
    ampXAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_AMP_OSCX, ampXSlider));
    freqXAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_FREQ_OSCX, freqXSlider));
    multXAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_MULT_OSCX, multXSlider));
    phaseXAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_PHASE_OSCX, phaseXSlider));
    ampYAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_AMP_OSCY, ampYSlider));
    freqYAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_FREQ_OSCY, freqYSlider));
    multYAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_MULT_OSCY, multYSlider));
    phaseYAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_PHASE_OSCY, phaseYSlider));
    dimAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_DIM, dimSlider));
    offxAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_OFFSETX, offXSlider));
    offyAttachment.reset(new SliderAttachment(audioProcessor.parameters, NAME_OFFSETY, offYSlider));
}

LissajousAudioProcessorEditor::~LissajousAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void LissajousAudioProcessorEditor::paint (juce::Graphics& g)
{
    int textH = 15;
    g.drawFittedText("Amp X", ampXSlider.getX(), ampXSlider.getY() - textH, ampXSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Freq X", freqXSlider.getX(), freqXSlider.getY() - textH, freqXSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Mult X", multXSlider.getX(), multXSlider.getY() - textH, multXSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Phase(pi) X", phaseXSlider.getX(), phaseXSlider.getY() - textH, phaseXSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Amp Y", ampYSlider.getX(), ampYSlider.getY() - textH, ampYSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Freq Y", freqYSlider.getX(), freqYSlider.getY() - textH, freqYSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Mult Y", multYSlider.getX(), multYSlider.getY() - textH, multYSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Phase(pi) Y", phaseYSlider.getX(), phaseYSlider.getY() - textH, phaseYSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Dimension", dimSlider.getX(), dimSlider.getY() - textH, dimSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Linked Freq", linkedSlider.getX(), linkedSlider.getY() - textH, linkedSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Velocity", velSlider.getX(), velSlider.getY() - textH, velSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("X DC-Offset", offXSlider.getX(), offXSlider.getY() - textH, offXSlider.getWidth(), textH, juce::Justification::centred, 1);
    g.drawFittedText("Y DC-Offset", offYSlider.getX(), offYSlider.getY() - textH, offYSlider.getWidth(), textH, juce::Justification::centred, 1);
}

void LissajousAudioProcessorEditor::resized()
{
    int sliderW = 100;
    int sliderH = 100;
    int spaceY = 25;
    scope.setBounds(getBounds().expanded(-10).withHeight(380));
    
    ampXSlider.setBounds(0, scope.getBottom() + spaceY, sliderW, sliderH);
    freqXSlider.setBounds(ampXSlider.getRight(), scope.getBottom() + spaceY, sliderW, sliderH);
    multXSlider.setBounds(freqXSlider.getRight(), scope.getBottom() + spaceY, sliderW, sliderH);
    phaseXSlider.setBounds(multXSlider.getRight(), scope.getBottom() + spaceY, sliderW, sliderH);
    
    ampYSlider.setBounds(0, ampXSlider.getBottom() + spaceY, sliderW, sliderH);
    freqYSlider.setBounds(ampYSlider.getRight(), ampXSlider.getBottom() + spaceY, sliderW, sliderH);
    multYSlider.setBounds(freqYSlider.getRight(), ampXSlider.getBottom() + spaceY, sliderW, sliderH);
    phaseYSlider.setBounds(multYSlider.getRight(), ampXSlider.getBottom() + spaceY, sliderW, sliderH);
    
    dimSlider.setBounds(0, ampYSlider.getBottom() + spaceY, sliderW, sliderH);
    linkedSlider.setBounds(dimSlider.getRight(), ampYSlider.getBottom() + spaceY, sliderW, sliderH);
    velSlider.setBounds(linkedSlider.getRight(), ampYSlider.getBottom() + spaceY, sliderW, sliderH);
    
    offXSlider.setBounds(velSlider.getRight(), ampYSlider.getBottom() + spaceY, sliderW, sliderH / 2);
    offYSlider.setBounds(velSlider.getRight(), ampYSlider.getBottom() + 2 * spaceY + sliderH / 2, sliderW, sliderH / 2);
    
}
