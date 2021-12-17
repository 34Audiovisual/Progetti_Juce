/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ControlerMidiAudioProcessorEditor::ControlerMidiAudioProcessorEditor (ControlerMidiAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(displayLab);
    displayLab.setJustificationType(juce::Justification::centred);
    displayLab.setText("SELECT DISPLAY", juce::dontSendNotification);
    
    addAndMakeVisible(positionLab);
    positionLab.setJustificationType(juce::Justification::centred);
    positionLab.setText("SELECT POSITION", juce::dontSendNotification);
    
    addAndMakeVisible(displayBox);
    displayBox.setColour(juce::ComboBox::backgroundColourId, juce::Colours::black);
    displayBox.addItemList(displayArray, 1);
    displayBox.setJustificationType(juce::Justification::centred);
    auto& valDisplayBox = displayBox.getSelectedIdAsValue();
    auto displayValue = audioProcessor.valuesTreeMidi.getPropertyAsValue("displayValue", nullptr, true);
    valDisplayBox.referTo(displayValue);
    
    addAndMakeVisible(positionBox);
    positionBox.setColour(juce::ComboBox::backgroundColourId, juce::Colours::black);
    positionBox.addItemList(positionArray, 1);
    positionBox.setJustificationType(juce::Justification::centred);
    auto& valPositionBox = positionBox.getSelectedIdAsValue();
    auto positionValue = audioProcessor.valuesTreeMidi.getPropertyAsValue("positionValue", nullptr, true);
    valPositionBox.referTo(positionValue);
    
    addAndMakeVisible(riga1);
    riga1.setJustification(juce::Justification::centred);
    riga1.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
    riga1.setTextToShowWhenEmpty("RIGA 1", juce::Colours::grey);
    riga1.setInputRestrictions(8);
    auto& valRiga1 = riga1.getTextValue();
    auto riga1Value = audioProcessor.valuesTreeMidi.getPropertyAsValue("riga1Value", nullptr, true);
    valRiga1.referTo(riga1Value);
    
    addAndMakeVisible(riga2);
    riga2.setJustification(juce::Justification::centred);
    riga2.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
    riga2.setTextToShowWhenEmpty("RIGA 2", juce::Colours::grey);
    riga2.setInputRestrictions(8);
    auto& valRiga2 = riga2.getTextValue();
    auto riga2Value = audioProcessor.valuesTreeMidi.getPropertyAsValue("riga2Value", nullptr, true);
    valRiga2.referTo(riga2Value);
    
    addAndMakeVisible(sentButton);
    sentButton.setButtonText ("Sent");
    sentButton.setToggleState(false, juce::dontSendNotification);
    auto& valSent = sentButton.getToggleStateValue();
    auto sentValue = audioProcessor.valuesTreeMidi.getPropertyAsValue("sentValue", nullptr, true);
    valSent.referTo(sentValue);

    
    addAndMakeVisible(send);
    send.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    send.setColour(juce::TextButton::buttonOnColourId, juce::Colours::white);
    send.setButtonText("SEND");
    send.onClick = [this]
    {
        auto displayChoose = displayBox.getSelectedId();
        auto positionChoose = positionBox.getSelectedId();
        auto riga1Text = riga1.getText();
        auto riga2Text = riga2.getText();

        if (displayChoose > 0 && positionChoose > 0)
        {
            startClicked(displayChoose, positionChoose, riga1Text, riga2Text);
            error.setVisible(false);
            sent.setVisible(true);
            sentButton.setToggleState(true, juce::sendNotification);
        }
        else
        {
            error.setVisible(true);
            sent.setVisible(false);
            sentButton.setToggleState(false, juce::sendNotification);
        }
    };
    
    addAndMakeVisible(error);
    error.setJustificationType(juce::Justification::centred);
    error.setColour(juce::Label::textColourId, juce::Colours::red);
    error.setColour(juce::Label::backgroundColourId, juce::Colours::yellow);
    error.setText("WRONG DATA! CHECK IT!", juce::dontSendNotification);
    error.setVisible(false);
    
    addAndMakeVisible(sent);
    sent.setJustificationType(juce::Justification::centred);
    sent.setColour(juce::Label::textColourId, juce::Colours::green);
    sent.setText("MESSAGE SENT", juce::dontSendNotification);
    sent.setVisible(false);

    
    auto tapImage = juce::ImageCache::getFromMemory (BinaryData::Logo_png, BinaryData::Logo_pngSize);
        
        if (tapImage.isValid())
            logo.setImage (tapImage, juce::RectanglePlacement::stretchToFit);
        else
            jassertfalse;
    addAndMakeVisible (logo);
    
    setSize (300, 500);
}

ControlerMidiAudioProcessorEditor::~ControlerMidiAudioProcessorEditor()
{
}

//==============================================================================
void ControlerMidiAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
    auto x = getWidth() * 0.1f;
    auto w = getWidth() * 0.8f;
    auto round = 20.f;
    auto thick = 2.f;
    auto border = 20;
    
    g.setColour (juce::Colours::black);
    g.fillAll();
    
    g.setColour (juce::Colours::lightyellow);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), round);
    
    g.setColour (juce::Colours::green);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), round, thick);
    
    g.fillRoundedRectangle(x, (float)(displayLab.getY()), w, (float)(displayLab.getHeight() + displayBox.getHeight() + border), round);
    g.fillRoundedRectangle(x, (float)(positionLab.getY()), w, (float)(positionLab.getHeight() + positionBox.getHeight() + border), round);
    g.fillRoundedRectangle(x, (float)(riga1.getY() - 10), w, (float)(riga1.getHeight() + riga2.getHeight() + 30), round);
    g.fillRoundedRectangle(x, (float)(send.getY() - 10), w, (float)(send.getHeight() + border), round);
   // g.setColour (juce::Colours::greenyellow);
    //g.fillRoundedRectangle(x, (float)(send.getBottom() + 30), w, (float)(send.getHeight() + 2 * border), round);

    
    g.setColour (juce::Colours::greenyellow);
    g.drawRoundedRectangle(x, (float)(displayLab.getY()), w, (float)(displayLab.getHeight() + displayBox.getHeight() + border), round, thick);
    g.drawRoundedRectangle(x, (float)(positionLab.getY()), w, (float)(positionLab.getHeight() + positionBox.getHeight() + border), round, thick);
    g.drawRoundedRectangle(x, (float)(riga1.getY() - 10), w, (float)(riga1.getHeight() + riga2.getHeight() + 30), round, thick);
    g.drawRoundedRectangle(x, (float)(send.getY() - 10), w, (float)(send.getHeight() + border), round, thick);
    g.drawRoundedRectangle(x, (float)(send.getBottom() + 30), w, (float)(send.getHeight() + 2 * border), round, thick*4);
    
    //g.setColour (juce::Colours::white);
    //g.drawFittedText("LOGO", 0, send.getBottom() + 50, getWidth(), 20, juce::Justification::centred, 1);
    
    g.setColour (juce::Colours::black);
    g.fillRoundedRectangle(displayBox.getX(), (float)(displayLab.getY() + 10 ), displayBox.getWidth(), (float)(displayLab.getHeight() - 20), round);
    g.fillRoundedRectangle(displayBox.getX(), (float)(positionLab.getY() + 10 ), displayBox.getWidth(), (float)(displayLab.getHeight() - 20), round);
}

void ControlerMidiAudioProcessorEditor::resized()
{
    auto w = getWidth() * 0.6;
    auto x = getWidth() * 0.2;
    auto h = 20;
    
    displayLab.setBounds(getLocalBounds().withY(10).withHeight(50));
    displayBox.setBounds(getWidth() * 0.2, displayLab.getBottom(), w, 20);
    
    positionLab.setBounds(getLocalBounds().withY(110).withHeight(50));
    positionBox.setBounds(x, positionLab.getBottom(), w, h);
    
    riga1.setBounds(x, positionBox.getBottom() + 60, w, h);
    riga2.setBounds(x, riga1.getBottom() + 10, w, h);
    
    send.setBounds(x, riga2.getBottom() + 60, w, h + 10);
    
    error.setBounds(x, getBottom() - 20, w, 15);
    sent.setBounds(x, getBottom() - 20, w, 15);
    
    logo.setBounds(getWidth() / 2 - 20, send.getBottom() + 42, 40, 50);
}


void ControlerMidiAudioProcessorEditor::startClicked (int display, int position, juce::String riga1T, juce::String riga2T)
{
    //spazi prima e dopo
    // “SPACE” “SPACE” “P” “I” “P” ”P”” O” “SPACE”
    // “SPACE” “SPACE” “M” “E” “L” ”A” “SPACE” “SPACE”
    auto r1 = makeString( riga1T );
    auto r2 = makeString( riga2T );
    riga1.setText(r1);
    riga2.setText(r2);

    noteArray1 = audioProcessor.makeNoteArray(r1);
    noteArray2 = audioProcessor.makeNoteArray(r2);
    
    audioProcessor.addEvent(display, position - 1, noteArray1, noteArray2);
}

juce::String ControlerMidiAudioProcessorEditor::makeString (juce::String string)
{
    juce::String stringToSend;
    
    int numSpazi = 8 - string.length() ;
    
    switch (numSpazi)
    {
        case 0:
            stringToSend = string;
            break;
        case 1:
            stringToSend = " " + string;
            break;
        case 2:
            stringToSend = " " + string + " ";
            break;
        case 3:
            stringToSend = "  " + string + " ";
            break;
        case 4:
            stringToSend = "  " + string + "  ";
            break;
        case 5:
            stringToSend = "   " + string + "  ";
            break;
        case 6:
            stringToSend = "   " + string + "   ";
            break;
        case 7:
            stringToSend = "    " + string + "   ";
            break;
        case 8:
            stringToSend = "        ";
            break;
            
        default:
            stringToSend = string;
            break;
    }
    
    return stringToSend;
}

