#pragma once
#include <JuceHeader.h>

#define BORDER_WIDTH 1.5f
#define KNOB_TO_TICK_RATIO 0.85
#define DEFAULT_NUM_TICKS 21

class MyLookAndFeel : public juce::LookAndFeel_V4
{
public:
	MyLookAndFeel()
	{
		//setColour(Slider::thumbColourId, Colours::red);
	}

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
	{
		// Variabili temporanee
		auto radius = (juce::jmin(width * 0.5f, height * 0.5f) - 2.0f * BORDER_WIDTH) * KNOB_TO_TICK_RATIO;
		auto centreX = x + width * 0.5f;
		auto centreY = y + height * 0.5f;
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;

		juce::Colour brightColour = juce::Colour(0xff2b2d31);
        juce::Colour darkColour = juce::Colour(0xff0d0d11);

		// Corpo della rotella
		//g.setColour(Colours::darkgrey);
		g.setGradientFill(juce::ColourGradient(brightColour, rx, ry, darkColour.brighter(0.05f), rx, ry + rw, false));
		g.fillEllipse(rx, ry, rw, rw);

		// Contorno rotella
		//g.setColour(Colours::black);
		g.setGradientFill(juce::ColourGradient(brightColour.brighter(0.2f), rx, ry, darkColour.darker(0.05f), rx, ry + rw, false));
		g.drawEllipse(rx, ry, rw, rw, BORDER_WIDTH);

		// Creo forma puntatore
		juce::Path p;
		auto pointerLength = radius * 0.33f;
		auto pointerThickness = 2.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);

		// Disegno il puntatore nelle coordinate corrette
		auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
		p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
		//g.setColour(Colours::white);
		g.setGradientFill(juce::ColourGradient(juce::Colours::white, rx, ry, juce::Colours::white.darker(0.2f), rx, ry + rw, false));
		g.fillPath(p);

		// Knob ticks
		juce::Path originalTickShape, t;
		auto tickSize = 2.0f;
		originalTickShape.addEllipse(-tickSize * 0.5f, -radius * (1.0f / KNOB_TO_TICK_RATIO), tickSize, tickSize);
		g.setColour(juce::Colours::grey);
		for (int i = 0; i < numTicks; ++i)
		{
			t = originalTickShape;
			float angle = rotaryStartAngle + pow(i / (numTicks - 1.0f), slider.getSkewFactor()) * (rotaryEndAngle - rotaryStartAngle);
			t.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
			g.fillPath(t);
		}
	}

	void setNumOfTicks(int newValue)
	{
		numTicks = newValue;
	}

	void drawToggleButton(juce::Graphics& gr, juce::ToggleButton& tb, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
	{

        juce::Rectangle<float> button;
        button.setBounds(0, 0, tb.getWidth(), tb.getHeight());
        
        gr.setColour(juce::Colours::grey);
        
        bool isOn = tb.getToggleState();
        
        if (isOn)
            gr.setColour(juce::Colours::green);
        else
            gr.setColour(juce::Colours::red);

        gr.fillRect(button);
	}

    /*
     
     void drawRotarySlider    (    juce::Graphics &     g,
                                                       int     x,
                                                       int     y,
                                                       int     width,
                                                       int     height,
                                                       float     sliderPosProportional,
                                                       float     rotaryStartAngle,
                                                       float     rotaryEndAngle,
                                juce::Slider &     slider ) override
      {

          juce::Image myStrip;

          myStrip = juce::ImageCache::getFromMemory (BinaryData::volumeKnob_png, BinaryData::volumeKnob_pngSize);
          
          
          const double fractRotation = (slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
          const int nFrames = myStrip.getHeight()/myStrip.getWidth(); // number of frames for vertical film strip
          const int frameIdx = (int)ceil(fractRotation * ((double)nFrames-1.0) ); // current index from 0 --> nFrames-1


          const float radius = juce::jmin (width / 2.0f, height / 2.0f) ;
          const float centreX = x + width * 0.5f;
          const float centreY = y + height * 0.5f;
          const float rx = centreX - radius - 1.0f;
                      const float ry = centreY - radius - 1.0f;

          g.drawImage(myStrip,
                      (int)rx,
                      (int)ry,
                      2*(int)radius,
                      2*(int)radius,   //Dest
                      0,
                      frameIdx*myStrip.getWidth(),
                      myStrip.getWidth(),
                      myStrip.getWidth()); //Source
          
      }
     */
private:
	int numTicks = DEFAULT_NUM_TICKS;
};
