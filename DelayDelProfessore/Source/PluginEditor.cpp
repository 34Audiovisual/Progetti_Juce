/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PluginEditor::PluginEditor (DelayEffectAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processor(p), valueTreeState(vts)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    timeSlider.reset (new juce::Slider ("Delay time"));
    addAndMakeVisible (timeSlider.get());
    timeSlider->setRange (0, 10, 0);
    timeSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    timeSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    timeSlider->setBounds (48, 56, 120, 136);

    fbSlider.reset (new juce::Slider ("Feedback"));
    addAndMakeVisible (fbSlider.get());
    fbSlider->setRange (0, 10, 0);
    fbSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    fbSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    fbSlider->setBounds (232, 56, 120, 136);

    dwSlider.reset (new juce::Slider ("Dry/Wet"));
    addAndMakeVisible (dwSlider.get());
    dwSlider->setRange (0, 10, 0);
    dwSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    dwSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    dwSlider->setBounds (416, 56, 120, 136);

    rateSlider.reset (new juce::Slider ("Mod rate"));
    addAndMakeVisible (rateSlider.get());
    rateSlider->setRange (0, 10, 0);
    rateSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    rateSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    rateSlider->setBounds (60, 251, 100, 117);

    amtSlider.reset (new juce::Slider ("Mod amount"));
    addAndMakeVisible (amtSlider.get());
    amtSlider->setRange (0, 10, 0);
    amtSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    amtSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    amtSlider->setBounds (244, 251, 100, 117);

    wfSlider.reset (new juce::Slider ("Mod shape"));
    addAndMakeVisible (wfSlider.get());
    wfSlider->setRange (0, 10, 0);
    wfSlider->setSliderStyle (juce::Slider::RotaryVerticalDrag);
    wfSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    wfSlider->setBounds (428, 251, 100, 117);


    //[UserPreSize]
    timeAttachment.reset(new SliderAttachment(valueTreeState, NAME_DELAY_TIME, *timeSlider));
    fbAttachment.reset(new SliderAttachment(valueTreeState, NAME_FB, *fbSlider));
    dwAttachment.reset(new SliderAttachment(valueTreeState, NAME_DW, *dwSlider));
    rateAttachment.reset(new SliderAttachment(valueTreeState, NAME_RATE, *rateSlider));
    amtAttachment.reset(new SliderAttachment(valueTreeState, NAME_AMT, *amtSlider));
    wfAttachment.reset(new SliderAttachment(valueTreeState, NAME_WF, *wfSlider));

    themeFloatParams.setNumOfTicks(21);
    themeWaveformParam.setNumOfTicks(4);

    //timeSlider->setLookAndFeel(&themeFloatParams);
    this->setLookAndFeel(&themeFloatParams);
    wfSlider->setLookAndFeel(&themeWaveformParam);

    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

PluginEditor::~PluginEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    this->setLookAndFeel(nullptr);

    timeAttachment.reset();
    fbAttachment.reset();
    dwAttachment.reset();
    rateAttachment.reset();
    amtAttachment.reset();
    wfAttachment.reset();
    //[/Destructor_pre]

    timeSlider = nullptr;
    fbSlider = nullptr;
    dwSlider = nullptr;
    rateSlider = nullptr;
    amtSlider = nullptr;
    wfSlider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void PluginEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = -1, y = 0, width = 601, height = 400;
        juce::Colour fillColour1 = juce::Colour (0xff2c2c32), fillColour2 = juce::Colour (0xff181719);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             280.0f - static_cast<float> (-1) + x,
                                             16.0f - 0.0f + y,
                                             fillColour2,
                                             352.0f - static_cast<float> (-1) + x,
                                             392.0f - 0.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = 55, y = 24, width = 105, height = 30;
        juce::String text (TRANS("Delay Time"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 239, y = 24, width = 105, height = 30;
        juce::String text (TRANS("Feedback"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 423, y = 24, width = 105, height = 30;
        juce::String text (TRANS("Dry / Wet"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 55, y = 224, width = 113, height = 30;
        juce::String text (TRANS("Mod rate"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 239, y = 224, width = 113, height = 30;
        juce::String text (TRANS("Mod amount"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 423, y = 224, width = 113, height = 30;
        juce::String text (TRANS("Waveform"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PluginEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PluginEditor" componentName=""
                 parentClasses="public juce::AudioProcessorEditor" constructorParams="DelayEffectAudioProcessor&amp; p, AudioProcessorValueTreeState&amp; vts"
                 variableInitialisers="AudioProcessorEditor(&amp;p), processor(p), valueTreeState(vts)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="-1 0 601 400" fill="linear: 280 16, 352 392, 0=ff2c2c32, 1=ff181719"
          hasStroke="0"/>
    <TEXT pos="55 24 105 30" fill="solid: ffffffff" hasStroke="0" text="Delay Time"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="239 24 105 30" fill="solid: ffffffff" hasStroke="0" text="Feedback"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="423 24 105 30" fill="solid: ffffffff" hasStroke="0" text="Dry / Wet"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="55 224 113 30" fill="solid: ffffffff" hasStroke="0" text="Mod rate"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="239 224 113 30" fill="solid: ffffffff" hasStroke="0" text="Mod amount"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="423 224 113 30" fill="solid: ffffffff" hasStroke="0" text="Waveform"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="Delay time" id="c5498ab9879058d4" memberName="timeSlider"
          virtualName="" explicitFocusOrder="0" pos="48 56 120 136" min="0.0"
          max="10.0" int="0.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Feedback" id="15760de0454a6df4" memberName="fbSlider" virtualName=""
          explicitFocusOrder="0" pos="232 56 120 136" min="0.0" max="10.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Dry/Wet" id="b557505c714cb78e" memberName="dwSlider" virtualName=""
          explicitFocusOrder="0" pos="416 56 120 136" min="0.0" max="10.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Mod rate" id="a71e5dc515008f4b" memberName="rateSlider"
          virtualName="" explicitFocusOrder="0" pos="60 251 100 117" min="0.0"
          max="10.0" int="0.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Mod amount" id="c30d9c7d3c179362" memberName="amtSlider"
          virtualName="" explicitFocusOrder="0" pos="244 251 100 117" min="0.0"
          max="10.0" int="0.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Mod shape" id="b0c02b96ada83ceb" memberName="wfSlider"
          virtualName="" explicitFocusOrder="0" pos="428 251 100 117" min="0.0"
          max="10.0" int="0.0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

