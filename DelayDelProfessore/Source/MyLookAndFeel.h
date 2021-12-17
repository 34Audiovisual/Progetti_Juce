#pragma once
#include <JuceHeader.h>

#define BORDER_WIDTH 1.5f
#define KNOB_TO_TICK_RATIO 0.85
#define DEFAULT_NUM_TICKS 21

class MyLookAndFeel : public LookAndFeel_V4
{
public:
	MyLookAndFeel()
	{
		//setColour(Slider::thumbColourId, Colours::red);
	}

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
		float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
	{
		// Variabili temporanee
		auto radius = (jmin(width * 0.5f, height * 0.5f) - 2.0f * BORDER_WIDTH) * KNOB_TO_TICK_RATIO;
		auto centreX = x + width * 0.5f;
		auto centreY = y + height * 0.5f;
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;

		Colour brightColour = Colour(0xff2b2d31);
		Colour darkColour = Colour(0xff0d0d11);

		// Corpo della rotella
		//g.setColour(Colours::darkgrey);
		g.setGradientFill(ColourGradient(brightColour, rx, ry, darkColour.brighter(0.05f), rx, ry + rw, false));
		g.fillEllipse(rx, ry, rw, rw);

		// Contorno rotella
		//g.setColour(Colours::black);
		g.setGradientFill(ColourGradient(brightColour.brighter(0.2f), rx, ry, darkColour.darker(0.05f), rx, ry + rw, false));
		g.drawEllipse(rx, ry, rw, rw, BORDER_WIDTH);

		// Creo forma puntatore
		Path p;
		auto pointerLength = radius * 0.33f;
		auto pointerThickness = 2.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);

		// Disegno il puntatore nelle coordinate corrette
		auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
		//g.setColour(Colours::white);
		g.setGradientFill(ColourGradient(Colours::white, rx, ry, Colours::white.darker(0.2f), rx, ry + rw, false));
		g.fillPath(p);

		// Knob ticks
		Path originalTickShape, t;
		auto tickSize = 2.0f;
		originalTickShape.addEllipse(-tickSize * 0.5f, -radius * (1.0f / KNOB_TO_TICK_RATIO), tickSize, tickSize);
		g.setColour(Colours::grey);
		for (int i = 0; i < numTicks; ++i)
		{
			t = originalTickShape;
			float angle = rotaryStartAngle + pow(i / (numTicks - 1.0f), slider.getSkewFactor()) * (rotaryEndAngle - rotaryStartAngle);
			t.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
			g.fillPath(t);
		}
	}

	void setNumOfTicks(int newValue)
	{
		numTicks = newValue;
	}

private:
	int numTicks = DEFAULT_NUM_TICKS;
};