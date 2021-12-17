/*
  ==============================================================================

    UI.cpp
    Created: 16 Nov 2021 4:01:56pm
    Author:  Diego Capoccitti

  ==============================================================================
*/

#include "UI.h"


#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
UI::UI (    juce::AudioProcessorValueTreeState& apvts )
    
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    timeSlider.reset (new juce::Slider ("Delay time"));
    addAndMakeVisible (timeSlider.get());
    timeSlider->setRange (0, 10, 0);
    timeSlider->setSliderStyle (juce::Slider::Rotary);
    timeSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    timeSlider->setBounds (216, 40, 168, 136);

    timeSliderL.reset (new juce::Slider ("Delay time left"));
    addAndMakeVisible (timeSliderL.get());
    timeSliderL->setRange (0, 10, 0);
    timeSliderL->setSliderStyle (juce::Slider::Rotary);
    timeSliderL->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    timeSliderL->setBounds (16, 40, 160, 136);

    timeSliderR.reset (new juce::Slider ("Delay time right"));
    addAndMakeVisible (timeSliderR.get());
    timeSliderR->setRange (0, 10, 0);
    timeSliderR->setSliderStyle (juce::Slider::Rotary);
    timeSliderR->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    timeSliderR->setBounds (424, 40, 160, 136);

    fbSlider.reset (new juce::Slider ("Feedback"));
    addAndMakeVisible (fbSlider.get());
    fbSlider->setRange (0, 10, 0);
    fbSlider->setSliderStyle (juce::Slider::Rotary);
    fbSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    fbSlider->setBounds (400, 248, 120, 104);

    dwSlider.reset (new juce::Slider ("Dry/wet"));
    addAndMakeVisible (dwSlider.get());
    dwSlider->setRange (0, 10, 0);
    dwSlider->setSliderStyle (juce::Slider::Rotary);
    dwSlider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);

    dwSlider->setBounds (80, 248, 120, 104);

    slrButton.reset (new juce::ToggleButton ("SLR"));
    addAndMakeVisible (slrButton.get());
    slrButton->setButtonText (juce::String());

    slrButton->setBounds (250, 264, 100, 64);


    //[UserPreSize]
    timeAttachment.reset(new SliderAttachment(apvts, NAME_DELAY_TIME, *timeSlider));
    timeLAttachment.reset(new SliderAttachment(apvts, NAME_DELAY_TIME_L, *timeSliderL));
    timeRAttachment.reset(new SliderAttachment(apvts, NAME_DELAY_TIME_R, *timeSliderR));
    fbAttachment.reset(new SliderAttachment(apvts, NAME_FB, *fbSlider));
    dwAttachment.reset(new SliderAttachment(apvts, NAME_DW, *dwSlider));
    slrAttachment.reset(new ButtonAttachment(apvts, NAME_CC, *slrButton));

    setLookAndFeel(&myTheme);

    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

UI::~UI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    this->setLookAndFeel(nullptr);
    timeAttachment.reset();
    timeLAttachment.reset();
    timeRAttachment.reset();
    fbAttachment.reset();
    dwAttachment.reset();
    slrAttachment.reset();
    //[/Destructor_pre]

    timeSlider = nullptr;
    timeSliderL = nullptr;
    timeSliderR = nullptr;
    fbSlider = nullptr;
    dwSlider = nullptr;
    slrButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UI::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    {
        int x = -2, y = 2, width = 602, height = 398;
        juce::Colour fillColour1 = juce::Colour (0xff004581), fillColour2 = juce::Colour (0xff001b5a);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setGradientFill (juce::ColourGradient (fillColour1,
                                             296.0f - static_cast<float> (-2) + x,
                                             96.0f - 2.0f + y,
                                             fillColour2,
                                             296.0f - static_cast<float> (-2) + x,
                                             288.0f - 2.0f + y,
                                             false));
        g.fillRect (x, y, width, height);
    }

    {
        int x = 230, y = 18, width = 136, height = 24;
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
        int x = 30, y = 18, width = 130, height = 24;
        juce::String text (TRANS("Delay Time L"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 438, y = 18, width = 136, height = 24;
        juce::String text (TRANS("Delay Time R"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 414, y = 226, width = 88, height = 24;
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
        int x = 94, y = 226, width = 88, height = 24;
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
        int x = 254, y = 226, width = 88, height = 24;
        juce::String text (TRANS("On = Stereo"));
        juce::Colour fillColour = juce::Colours::white;
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
        g.drawText (text, x, y, width, height,
                    juce::Justification::centred, true);
    }

    {
        int x = 246, y = 338, width = 114, height = 24;
        juce::String text (TRANS("Off = Left / Right"));
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

void UI::resized()
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
                 parentClasses="public juce::AudioProcessorEditor" constructorParams="DelayAudioProcessor&amp; p, AudioProcessorValueTreeState&amp; vts"
                 variableInitialisers="AudioProcessorEditor(&amp;p),&#10;processor(p), valueTreeState(vts)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <RECT pos="-2 2 602 398" fill="linear: 296 96, 296 288, 0=ff004581, 1=ff001b5a"
          hasStroke="0"/>
    <TEXT pos="230 18 136 24" fill="solid: ffffffff" hasStroke="0" text="Delay Time"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="30 18 130 24" fill="solid: ffffffff" hasStroke="0" text="Delay Time L"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="438 18 136 24" fill="solid: ffffffff" hasStroke="0" text="Delay Time R"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="414 226 88 24" fill="solid: ffffffff" hasStroke="0" text="Feedback"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="94 226 88 24" fill="solid: ffffffff" hasStroke="0" text="Dry / Wet"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="254 226 88 24" fill="solid: ffffffff" hasStroke="0" text="On = Stereo"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="246 338 114 24" fill="solid: ffffffff" hasStroke="0" text="Off = Left / Right"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="Delay time" id="d38973dcedfd374" memberName="timeSlider"
          virtualName="" explicitFocusOrder="0" pos="216 40 168 136" min="0.0"
          max="10.0" int="0.0" style="Rotary" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Delay time left" id="92fbc1eba8ffead8" memberName="timeSliderL"
          virtualName="" explicitFocusOrder="0" pos="16 40 160 136" min="0.0"
          max="10.0" int="0.0" style="Rotary" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Delay time right" id="ec46f3489ba63c2" memberName="timeSliderR"
          virtualName="" explicitFocusOrder="0" pos="424 40 160 136" min="0.0"
          max="10.0" int="0.0" style="Rotary" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="0"/>
  <SLIDER name="Feedback" id="f1088ebec442d040" memberName="fbSlider" virtualName=""
          explicitFocusOrder="0" pos="400 248 120 104" min="0.0" max="10.0"
          int="0.0" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="0"/>
  <SLIDER name="Dry/wet" id="a34ba3409f67baad" memberName="dwSlider" virtualName=""
          explicitFocusOrder="0" pos="80 248 120 104" min="0.0" max="10.0"
          int="0.0" style="Rotary" textBoxPos="TextBoxBelow" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="0"/>
  <TOGGLEBUTTON name="SLR" id="b4abbae53fe1b7a0" memberName="slrButton" virtualName=""
                explicitFocusOrder="0" pos="200 264 200 64" buttonText="" connectedEdges="0"
                needsCallback="0" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

